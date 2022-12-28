#ifndef COMMANDS_S2_H
#define COMMANDS_S2_H

#include <iosfwd>
#include <string>
#include "IODataset.h"
#include "Dictionary.h"

namespace ivlicheva
{
  struct Commands
  {
    using dictionary_iter_t = dictionary_t::iterator_t;

    Commands(const dictionaries_t& dictionaries, std::ostream& ostream);
    void output(std::string str);
    void complement(std::string str);
    void intersect(std::string str);
    void unite(std::string str);

    private:
      dictionaries_t dictionaries_;
      std::ostream& ostream_;
      void doPrint(dictionary_t& dict);
      void doComplement(const std::string& str, dictionary_t& dict1, dictionary_t& dict2);
      void doIntersect(const std::string& str, dictionary_t& dict1, dictionary_t& dict2);
      void doUnion(const std::string& str, dictionary_t& dict1, dictionary_t& dict2);
  };
}

#endif
