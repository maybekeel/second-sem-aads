#include <iostream>
#include <cstdlib>
#include <deque>
#include <string>
#include <ctime>
#include <iomanip>
#include "ForwardList.h"
#include "BidirectionalList.h"
#include "sortings.h"

namespace
{
  template< typename T >
  void print(std::ostream& stream, const T& list)
  {
    bool isFirst = 1;
    for (auto&& i: list)
    {
      if (!isFirst)
      {
        stream << ' ' << i;
      }
      else
      {
        stream << i;
        isFirst = 0;
      }
    }
    stream << '\n';
  }

  template< typename T, typename Compare >
  void getResult(std::ostream& stream, int size, Compare cmp)
  {
    ivlicheva::ForwardList< T > forwardList;
    ivlicheva::BidirectionalList< T > bidirectionalList;
    std::deque< T > deque;

    for (int i = 0; i < size; ++i)
    {
      T value = std::rand();
      forwardList.pushBack(value);
      bidirectionalList.pushBack(value);
      deque.push_back(value);
      stream << value;
      if (i != size - 1)
      {
        stream << ' ';
      }
    }
    stream << '\n';

    ivlicheva::timsort(deque.begin(), size, cmp);
    print(stream, deque);

    ivlicheva::odd_even(deque.begin(), deque.end(), cmp);
    print(stream, deque);

    ivlicheva::bucket(deque.begin(), deque.end(), cmp);
    print(stream, deque);

    ivlicheva::odd_even(bidirectionalList.begin(), bidirectionalList.end(), cmp);
    print(stream, bidirectionalList);

    ivlicheva::timsort(bidirectionalList.begin(), size, cmp);
    print(stream, bidirectionalList);

    ivlicheva::bucket(forwardList.begin(), forwardList.end(), cmp);
    print(stream, forwardList);
  }
}

int main(int argc, char** argv)
{
  std::srand(std::time(0));
  if (argc != 4)
  {
    std::cerr << "bad args\n";
    return 1;
  }
  std::string sortType = argv[1];
  if (sortType != "ascending" && sortType != "descending")
  {
    std::cerr << "bad type of sort\n";
    return 1;
  }
  std::string valueType = argv[2];
  if (valueType != "ints" && valueType != "floats")
  {
    std::cerr << "bad type of values\n";
    return 1;
  }
  std::string sizeSTR = argv[3];
  int size = std::stoi(sizeSTR);
  if (size <= 0)
  {
    std::cerr << "bad size\n";
    return 1;
  }
  if (valueType == "ints")
  {
    if (sortType == "ascending")
    {
      getResult< int >(std::cout, size, std::less< int >());
    }
    else if (sortType == "descending")
    {
      getResult< int >(std::cout, size, std::greater< int >());
    }
  }
  else if (valueType == "floats")
  {
    std::cout << std::fixed << std::setprecision(1);
    if (sortType == "ascending")
    {
      getResult< float >(std::cout, size, std::less< float >());
    }
    else if (sortType == "descending")
    {
      getResult< float >(std::cout, size, std::greater< float >());
    }
  }
  return 0;
}
