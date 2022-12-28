#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include "BinarySearchTree.h"
#include "iofunctional.h"
#include "summation.h"
#include "iotree.h"

namespace
{
  class IOsum
  {
    public:
      void operator()(const std::pair< long long, std::string >&);
      friend std::ostream& operator<<(std::ostream&, const IOsum&);

    private:
      ivlicheva::Sum sum_;
      ivlicheva::Print print_;
  };

  std::ostream& operator<<(std::ostream& stream, const IOsum& result)
  {
    return stream << result.sum_.getSum() << ' ' << result.print_.getTree();
  }

  void IOsum::operator()(const std::pair< long long, std::string >& data)
  {
    sum_(data);
    print_(data);
  }
}

int main(int argc, char** argv)
{
  if (argc != 3)
  {
    std::cerr << "bad args\n";
    return 1;
  }
  std::string arg = argv[1];
  if (arg != "ascending" && arg != "descending" && arg != "breadth")
  {
    std::cerr << "bad arg\n";
    return 1;
  }
  std::ifstream file(argv[2]);
  if (!file.is_open())
  {
    std::cerr << "file is not open\n";
    return 1;
  }
  ivlicheva::BinarySearchTree< long long, std::string, std::less< long long > > tree = ivlicheva::readTreeFromStream(file);
  file.close();
  if (tree.isEmpty())
  {
    std::cout << "<EMPTY>\n";
    return 0;
  }
  IOsum result;
  try
  {
    if (arg == "ascending")
    {
      result = tree.traverseLNR(result);
    }
    else if (arg == "descending")
    {
      result = tree.traverseRNL(result);
    }
    else if (arg == "breadth")
    {
      result = tree.traverseBreadth(result);
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return 2;
  }
  std::cout << result << '\n';
  return 0;
}
