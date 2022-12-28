#include "Commands.h"
#include <iostream>
#include <fstream>
#include <string>
#include "Dictionary.h"
#include "BidirectionalList.h"
#include "iolists.h"
#include "IOParse.h"
#include "iomessages.h"

namespace
{
  bool isNumber(std::string s)
  {
    bool isNumber = true;
    for (size_t i = 0; i < s.size() && isNumber; i++)
    {
      isNumber = isNumber && std::isdigit(s.at(i));
    }
    return isNumber;
  }
}

ivlicheva::Commands::Commands(const lists_t& lists, std::ostream& ostream):
  lists_(lists),
  ostream_(ostream)
{}

void ivlicheva::Commands::print(std::string str)
{
  std::string name = getSubstring(str);
  if (str.empty())
  {
    list_t dict = lists_.get(name);
    if (dict.isEmpty())
    {
      ivlicheva::outputMessageEmpty(ostream_);
    }
    else
    {
      ostream_ << name;
      doPrint(dict);
    }
  }
}

void ivlicheva::Commands::doPrint(list_t& list)
{
  list_iter_t iter = list.begin();
  while (iter != list.end())
  {
    ostream_ << ' ' << *iter;
    ++iter;
  }
  ostream_ << '\n';
}

void ivlicheva::Commands::replace(std::string str)
{
  std::string name = getSubstring(str);
  std::string arg1 = getSubstring(str);
  std::string arg2 = getSubstring(str);
  if (name.empty() || arg1.empty() || arg2.empty())
  {
    throw std::logic_error("Bad args");
  }
  list_t& list = lists_.get(name);
  long long from = std::stoll(arg1);
  if (isNumber(arg2))
  {
    doReplace(list, from, std::stoll(arg2));
  }
  else
  {
    list_t list2 = lists_.get(arg2);
    doReplace(list, from, list2);
  }
}

void ivlicheva::Commands::remove(std::string str)
{
  std::string name = getSubstring(str);
  std::string arg1 = getSubstring(str);
  list_t& list = lists_.get(name);
  if (isNumber(arg1))
  {
    doRemove(list, std::stoll(arg1));
  }
  else
  {
    list_t list2 = lists_.get(arg1);
    doRemove(list, list2);
  }
}

void ivlicheva::Commands::concat(std::string str)
{
  std::string nameNew = getSubstring(str);
  size_t n = 0;
  list_t list;
  list_t::Iterator iter = list.beforeBegin();
  while (!str.empty())
  {
    std::string arg = getSubstring(str);
    list_t list2 = lists_.get(arg);
    for (auto&& i: list2)
    {
      list.pushAfter(i, iter);
      ++iter;
    }
    ++n;
  }
  if (n < 2)
  {
    throw std::logic_error("Not enough args");
  }
  lists_.push(nameNew, list);
}

void ivlicheva::Commands::equal(std::string str)
{
  size_t n = 0;
  std::string name1;
  std::string name2;
  while (!str.empty())
  {
    if (n == 0)
    {
      name1 = getSubstring(str);
    }
    else
    {
      name1 = name2;
    }
    name2 = getSubstring(str);
    list_t list1 = lists_.get(name1);
    list_t list2 = lists_.get(name2);
    if (list1 != list2)
    {
      outputMessageFalse(ostream_);
      return;
    }
    ++n;
  }
  outputMessageTrue(ostream_);
}

void ivlicheva::Commands::doReplace(list_t& list, long long from, long long to)
{
  for (auto& i: list)
  {
    if (i == from)
    {
      i = to;
    }
  }
}

void ivlicheva::Commands::doReplace(list_t& list, long long from, const list_t& to)
{
  list_t::Iterator iter = list.begin();
  while (iter != list.end())
  {
    if (*iter == from)
    {
      list_t::Iterator iter2 = to.begin();
      *iter = *iter2;
      ++iter2;
      while (iter2 != to.end())
      {
        list.pushAfter(*iter2, iter);
        ++iter;
        ++iter2;
      }
    }
    ++iter;
  }
}

void ivlicheva::Commands::doRemove(list_t& list, long long value)
{
  list_t::Iterator iter = list.begin();
  list_t::Iterator iter2 = list.beforeBegin();
  while (iter != list.end())
  {
    if (*iter == value)
    {
      ++iter;
      list.dropAfter(iter2);
    }
    else
    {
      ++iter;
      ++iter2;
    }
  }
}

void ivlicheva::Commands::doRemove(list_t& list, const list_t& list2)
{
  for (auto&& i: list2)
  {
    doRemove(list, i);
  }
}
