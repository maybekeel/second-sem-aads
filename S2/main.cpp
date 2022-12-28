#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include "Dictionary.h"
#include "IODataset.h"
#include "iomessages.h"
#include "CommandsS2.h"
#include "IOParse.h"

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
  ivlicheva::Commands funcs(ivlicheva::readDictionariesFromFile(file), std::cout);
  file.close();

  ivlicheva::Dictionary< std::string, command_t, std::less< std::string > > dictionaryOfCommands(
   {
     {"print", std::bind(&ivlicheva::Commands::output, &funcs, std::placeholders::_1)},
     {"complement", std::bind(&ivlicheva::Commands::complement, &funcs, std::placeholders::_1)},
     {"intersect", std::bind(&ivlicheva::Commands::intersect, &funcs, std::placeholders::_1)},
     {"union", std::bind(&ivlicheva::Commands::unite, &funcs, std::placeholders::_1)}
   });

  while (!std::cin.eof())
  {
    std::string command = "";
    std::getline(std::cin, command);
    if (command.size())
    {
      std::string name = ivlicheva::getSubstring(command);
      try
      {
        dictionaryOfCommands.get(name)(command);
      }
      catch (const std::exception&)
      {
        ivlicheva::outputMessageInvalidCommand(std::cout);
      }
    }
  }
  return 0;
}
