#ifndef IOLISTS_H
#define IOLISTS_H

#include <fstream>
#include <string>
#include "BidirectionalList.h"
#include "Dictionary.h"

namespace ivlicheva
{
  using list_t = BidirectionalList< long long >;
  using dictionary_t = Dictionary< std::string, list_t, std::less< std::string > >;
  dictionary_t readListsFromFile(std::ifstream&);
}

#endif
