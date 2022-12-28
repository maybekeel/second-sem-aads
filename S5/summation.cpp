#include "summation.h"
#include <limits>
#include <stdexcept>

ivlicheva::Sum::Sum():
  sum_(0)
{}

void ivlicheva::Sum::Sum::operator()(const std::pair< long long, std::string >& data)
{
  if (sum_ > 0 && data.first > 0)
  {
    const long long max_int = std::numeric_limits< long long >::max();
    if (max_int - sum_ < data.first)
    {
      throw std::overflow_error("Overflow");
    }
  }
  if (sum_ < 0 && data.first < 0)
  {
    const long long min_int = std::numeric_limits< long long >::min();
    if (min_int - sum_ > data.first)
    {
      throw std::overflow_error("Overflow");
    }
  }
  sum_ += data.first;
}

long long ivlicheva::Sum::getSum() const
{
  return sum_;
}
