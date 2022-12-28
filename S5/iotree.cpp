#include "iotree.h"
#include <iostream>

ivlicheva::BinarySearchTree< long long, std::string, std::less< long long > > ivlicheva::readTreeFromStream(std::istream& stream)
{
  BinarySearchTree< long long, std::string, std::less< long long > > tree;
  while (!stream.eof() && !stream.fail())
  {
    long long k1 = 0;
    std::string k2 = "";
    stream >> k1 >> k2;
    if (!stream.fail())
    {
      tree.push(k1, k2);
    }
  }
  return tree;
}
