#include "IODataset.h"
#include <fstream>
#include <string>
#include "Dictionary.h"
#include "IOParse.h"

namespace
{
  ivlicheva::dictionary_t splitStringToDictionary(std::string str)
  {
    ivlicheva::dictionary_t dictionary;
    if (str.empty())
    {
      throw std::logic_error("Bad dictionary");
    }
    while (str.size())
    {
      std::string keyS = ivlicheva::getSubstring(str);
      if (str.empty())
      {
        break;
      }
      int key = std::stoi(keyS);
      std::string data = ivlicheva::getSubstring(str);
      dictionary.push(key, data);
    }
    return dictionary;
  }
}

ivlicheva::dictionaries_t ivlicheva::readDictionariesFromFile(std::istream& file)
{
  dictionaries_t dictionaries;
  while (!file.eof())
  {
    std::string str = "";
    std::getline(file, str);
    if (str.size())
    {
      std::string name = getSubstring(str);
      if (str.empty())
      {
        dictionaries.push(name, dictionary_t{});
      }
      else if (str.size())
      {
        dictionaries.push(name, splitStringToDictionary(str));
      }
    }
  }
  return dictionaries;
}
