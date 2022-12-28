#ifndef SUMMATION_H
#define SUMMATION_H

#include <utility>
#include <string>

namespace ivlicheva
{
  class Sum
  {
    public:
      Sum();
      void operator()(const std::pair< long long, std::string >&);
      long long getSum() const;

    private:
      long long sum_;
  };
}

#endif
