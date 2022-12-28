#include "IOParse.h"
#include <string>

std::string ivlicheva::getSubstring(std::string& str)
{
  size_t n = str.find_first_of(' ', 0);
  std::string word = str.substr(0, n);
  n = (n == str.npos) ? n : n + 1;
  str.erase(0, n);
  return word;
}
