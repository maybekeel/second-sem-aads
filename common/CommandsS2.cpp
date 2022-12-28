#include "CommandsS2.h"
#include <iostream>
#include <fstream>
#include <string>
#include "Dictionary.h"
#include "iomessages.h"
#include "IOParse.h"

namespace
{
  void checkAndPushDict(ivlicheva::dictionaries_t& dictionaries, ivlicheva::dictionary_t& newDictionary, const std::string& str)
  {
    try
    {
      dictionaries.get(str) = newDictionary;
    }
    catch (const std::exception&)
    {
      dictionaries.push(str, newDictionary);
    }
  }
}

ivlicheva::Commands::Commands(const dictionaries_t& dictionaries, std::ostream& ostream):
  dictionaries_(dictionaries),
  ostream_(ostream)
{}

void ivlicheva::Commands::output(std::string str)
{
  std::string name = getSubstring(str);
  if (str.empty())
  {
    dictionary_t dict = dictionaries_.get(name);
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

void ivlicheva::Commands::complement(std::string str)
{
  std::string arg1 = getSubstring(str);
  std::string arg2 = getSubstring(str);
  std::string arg3 = getSubstring(str);
  dictionary_t firstDict = dictionaries_.get(arg2);
  dictionary_t secondDict = dictionaries_.get(arg3);
  doComplement(arg1, firstDict, secondDict);
}

void ivlicheva::Commands::intersect(std::string str)
{
  std::string arg1 = getSubstring(str);
  std::string arg2 = getSubstring(str);
  std::string arg3 = getSubstring(str);
  dictionary_t firstDict = dictionaries_.get(arg2);
  dictionary_t secondDict = dictionaries_.get(arg3);
  doIntersect(arg1, firstDict, secondDict);
}

void ivlicheva::Commands::unite(std::string str)
{
  std::string arg1 = getSubstring(str);
  std::string arg2 = getSubstring(str);
  std::string arg3 = getSubstring(str);
  dictionary_t firstDict = dictionaries_.get(arg2);
  dictionary_t secondDict = dictionaries_.get(arg3);
  doUnion(arg1, firstDict, secondDict);
}

void ivlicheva::Commands::doPrint(dictionary_t& dict)
{
  dictionary_iter_t iter = dict.begin();
  while (iter != dict.end())
  {
    ostream_ << ' ' << iter->first << ' ' << iter->second;
    ++iter;
  }
  ostream_ << '\n';
}

void ivlicheva::Commands::doComplement(const std::string& str, dictionary_t& dict1, dictionary_t& dict2)
{
  dictionary_t newDictionary;
  for (auto&& i: dict1)
  {
    int c = 0;
    for (auto&& j: dict2)
    {
      if (i.first == j.first)
      {
        ++c;
      }
    }
    if (c == 0)
    {
      newDictionary.push(i.first, i.second);
    }
  }
  checkAndPushDict(dictionaries_, newDictionary, str);
}

void ivlicheva::Commands::doIntersect(const std::string& str, dictionary_t& dict1, dictionary_t& dict2)
{
  dictionary_t newDictionary;
  for (auto&& i: dict1)
  {
    for (auto&& j: dict2)
    {
      if (i.first == j.first)
      {
        newDictionary.push(i.first, i.second);
        break;
      }
    }
  }
  checkAndPushDict(dictionaries_, newDictionary, str);
}

void ivlicheva::Commands::doUnion(const std::string& str, dictionary_t& dict1, dictionary_t& dict2)
{
  dictionary_t newDictionary;
  for (auto&& i: dict1)
  {
    newDictionary.push(i.first, i.second);
  }
  for (auto&& i: dict2)
  {
    int c = 0;
    for (auto&& j: dict1)
    {
      if (i.first == j.first)
      {
        ++c;
      }
    }
    if (c == 0)
    {
      newDictionary.push(i.first, i.second);
    }
  }
  checkAndPushDict(dictionaries_, newDictionary, str);
}
