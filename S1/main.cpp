#include <iostream>
#include <fstream>
#include <string>
#include "Queue.h"
#include "Stack.h"
#include "Funcs.h"

int main(int argc, char** argv)
{
  ivlicheva::Queue< std::string > queue;
  if (argc == 2)
  {
    std::string fileName = argv[1];
    std::ifstream file(fileName);
    if (file.is_open())
    {
      queue = ivlicheva::readFromStream(file);
    }
    else
    {
      std::cerr << "Error. File is not open.";
    }
    file.close();
    if (!file)
    {
      std::cerr << "Error. File is open";
    }
  }
  else if (argc == 1)
  {
    queue = ivlicheva::readFromStream(std::cin);
  }
  else
  {
    std::cerr << "Bad args\n";
    return 1;
  }
  try
  {
    ivlicheva::Stack< long long > stack;
    stack = ivlicheva::convertExpressions(queue);
    ivlicheva::outputExpressions(std::cout, stack);
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return 2;
  }

  return 0;
}
