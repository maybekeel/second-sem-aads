#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <limits>
#include <string>
#include <functional>
#include <map>
#include "commands.hpp"
#include "matrix.hpp"

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    std::cerr << "Bad args\n";
    return 1;
  }
  std::ifstream file(argv[1]);
  if (!file.is_open())
  {
    std::cerr << "File is not open\n";
    return 1;
  }
  std::map< std::string, ivlicheva::Matrix< int > > matrices;
  while (!file.eof())
  {
    std::string matrixName = "";
    file >> matrixName;
    if (!matrixName.empty())
    {
      matrices.insert({matrixName, ivlicheva::Matrix< int >(file)});
    }
    if (!file)
    {
      file.clear();
      file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  file.close();
  ivlicheva::Commands commands(matrices, std::cin, std::cout);
  using command_t = std::function< bool(void) >;
  std::map< std::string, command_t > mapOfCommands(
   {
     {"create", std::bind(&ivlicheva::Commands::create, std::ref(commands))},
     {"input", std::bind(&ivlicheva::Commands::input, std::ref(commands))},
     {"fill", std::bind(&ivlicheva::Commands::fill, std::ref(commands))},
     {"mult", std::bind(&ivlicheva::Commands::mult, std::ref(commands))},
     {"sum", std::bind(&ivlicheva::Commands::sum, std::ref(commands))},
     {"det", std::bind(&ivlicheva::Commands::getDeterminant, std::ref(commands))},
     {"sub", std::bind(&ivlicheva::Commands::substract, std::ref(commands))},
     {"hconcat", std::bind(&ivlicheva::Commands::hconcat, std::ref(commands))},
     {"vconcat", std::bind(&ivlicheva::Commands::vconcat, std::ref(commands))},
     {"hrepeat", std::bind(&ivlicheva::Commands::hrepeat, std::ref(commands))},
     {"vrepeat", std::bind(&ivlicheva::Commands::vrepeat, std::ref(commands))},
     {"repeat", std::bind(&ivlicheva::Commands::repeat, std::ref(commands))},
     {"print", std::bind(&ivlicheva::Commands::print, std::ref(commands))}
   });
  while (!std::cin.eof())
  {
    std::string nameOfCommand = "";
    std::cin >> nameOfCommand;
    if (!nameOfCommand.empty())
    {
      auto commandIter = mapOfCommands.find(nameOfCommand);
      if (commandIter == mapOfCommands.end() || !commandIter->second())
      {
        std::cout << "<INVALID COMMAND>\n";
        if (!std::cin)
        {
          std::cin.clear();
          std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
        }
      }
    }
  }
  return 0;
}
