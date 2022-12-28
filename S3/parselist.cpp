#include "parselist.h"
#include <string>
#include <stdexcept>
#include "IOParse.h"

ivlicheva::list_t ivlicheva::splitStringToList(std::string& str)
{
  list_t list;
  if (str.empty())
  {
    return list;
  }
  list_t::Iterator iter = list.beforeBegin();
  while (!str.empty())
  {
    std::string newNumber = getSubstring(str);
    list.pushAfter(std::stoll(newNumber), iter);
    ++iter;
  }
  return list;
}
