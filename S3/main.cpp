#include <iostream>
#include <functional>
#include <fstream>
#include "BidirectionalList.h"
#include "Dictionary.h"
#include "iolists.h"
#include "Commands.h"
#include "IOParse.h"
#include "parselist.h"
#include "iomessages.h"

int main(int argc, char** argv)
{
  using command_t = std::function< void(std::string) >;

  if (argc != 2)
  {
    std::cerr << "Error\n";
    return 1;
  }
  std::ifstream file(argv[1]);
  if (!file.is_open())
  {
    std::cerr << "File is not open\n";
    return 1;
  }
  ivlicheva::Commands funcs(ivlicheva::readListsFromFile(file), std::cout);
  file.close();

  ivlicheva::Dictionary< std::string, command_t, std::less< std::string > > dictionaryOfCommands(
    {
      {"print", std::bind(&ivlicheva::Commands::print, &funcs, std::placeholders::_1)},
      {"replace", std::bind(&ivlicheva::Commands::replace, &funcs, std::placeholders::_1)},
      {"remove", std::bind(&ivlicheva::Commands::remove, &funcs, std::placeholders::_1)},
      {"concat", std::bind(&ivlicheva::Commands::concat, &funcs, std::placeholders::_1)},
      {"equal", std::bind(&ivlicheva::Commands::equal, &funcs, std::placeholders::_1)}
    });

  while (!std::cin.eof())
  {
    std::string command = "";
    getline(std::cin, command);
    if (command.size())
    {
      size_t n = command.find_first_of(' ', 0);
      std::string arguments = command.substr(n + 1);
      try
      {
        dictionaryOfCommands.get(command.substr(0, n))(arguments);
      }
      catch (const std::exception&)
      {
        ivlicheva::outputMessageInvalidCommand(std::cout);
      }
    }
  }
  return 0;
}
