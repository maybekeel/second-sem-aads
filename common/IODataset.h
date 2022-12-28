#ifndef IODATASET_H
#define IODATASET_H

#include <string>
#include <iosfwd>
#include "Dictionary.h"

namespace ivlicheva
{
  using dictionary_t = Dictionary< int, std::string, std::less< int > >;
  using dictionaries_t = Dictionary< std::string, dictionary_t, std::less< std::string > >;
  dictionaries_t readDictionariesFromFile(std::istream& file);
}

#endif
