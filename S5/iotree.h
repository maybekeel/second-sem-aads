#ifndef IOTREE_H
#define IOTREE_H

#include <iosfwd>
#include <string>
#include "BinarySearchTree.h"

namespace ivlicheva
{
  BinarySearchTree< long long, std::string, std::less< long long > > readTreeFromStream(std::istream&);
}

#endif
