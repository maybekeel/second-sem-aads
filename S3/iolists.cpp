#include "iolists.h"
#include <fstream>
#include <string>
#include "BidirectionalList.h"
#include "Dictionary.h"
#include "IOParse.h"
#include "parselist.h"

ivlicheva::dictionary_t ivlicheva::readListsFromFile(std::ifstream& file)
{
  dictionary_t dictionary;
  while (!file.eof())
  {
    std::string str = "";
    std::getline(file, str);
    if (str.size())
    {
      std::string name = getSubstring(str);
      dictionary.push(name, splitStringToList(str));
    }
  }
  return dictionary;
}
