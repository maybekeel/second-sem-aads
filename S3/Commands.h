#ifndef COMMANDS_H
#define COMMANDS_H

#include <string>
#include "Dictionary.h"
#include "BidirectionalList.h"

namespace ivlicheva
{
  struct Commands
  {
    using list_t = BidirectionalList< long long >;
    using lists_t = Dictionary< std::string, list_t, std::less< std::string > >;
    using list_iter_t = list_t::Iterator;

    Commands(const lists_t&, std::ostream&);
    void print(std::string);
    void replace(std::string);
    void remove(std::string);
    void concat(std::string);
    void equal(std::string);

  private:
    lists_t lists_;
    std::ostream& ostream_;

    void doPrint(list_t&);
    void doReplace(list_t&, long long, long long);
    void doReplace(list_t&, long long, const list_t&);
    void doRemove(list_t&, long long);
    void doRemove(list_t&, const list_t&);
  };
}

#endif
