#ifndef IOFUNCTIONAL_H
#define IOFUNCTIONAL_H

#include <string>
#include <utility>

namespace ivlicheva
{
  class Print
  {
    public:
      void operator()(const std::pair< long long, std::string >&);
      const std::string& getTree() const;

    private:
      std::string tree_;
  };
}

#endif
