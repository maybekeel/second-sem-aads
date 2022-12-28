#ifndef SORTINGS_H
#define SORTINGS_H

#include <cstddef>
#include <iterator>
#include <cmath>
#include "Dictionary.h"
#include "BidirectionalList.h"
#include "Queue.h"

namespace ivlicheva
{
  namespace detail
  {
    template< typename BidIterator, typename Compare >
    void insertionsort(BidIterator, BidIterator, Compare);
    template< typename FwdIterator, typename Compare >
    void mergesort(FwdIterator, FwdIterator, FwdIterator, FwdIterator, Compare);
    template< typename FwdIterator, typename Compare >
    FwdIterator getMax(FwdIterator, FwdIterator, Compare);
    template< typename FwdIterator, typename Compare >
    FwdIterator getMin(FwdIterator, FwdIterator, Compare);
  }
}

template< typename BidIterator, typename Compare >
void ivlicheva::detail::insertionsort(BidIterator first, BidIterator last, Compare cmp)
{
  if (first == last)
  {
    return;
  }
  BidIterator tmpFirst = first;
  if (++tmpFirst == last)
  {
    return;
  }
  for (BidIterator i = tmpFirst; i != last; ++i)
  {
    for (BidIterator j = i; j != first; --j)
    {
      BidIterator tmp = j;
      if (!cmp(*(--tmp), *j))
      {
        std::iter_swap(tmp, j);
      }
    }
  }
}

template< typename FwdIterator, typename Compare >
void ivlicheva::detail::mergesort(FwdIterator first1, FwdIterator last1, FwdIterator first2, FwdIterator last2, Compare cmp)
{
  static_assert(!std::is_same< typename std::iterator_traits< FwdIterator >::value_type, void >::value);
  Queue< typename FwdIterator::value_type > queue;
  FwdIterator begin1 = first1;
  FwdIterator end1 = last1;
  FwdIterator begin2 = first2;
  FwdIterator end2 = last2;
  while (begin1 != end1 || begin2 != end2)
  {
    if (cmp(*begin1, *begin2))
    {
      queue.push(std::move(*begin1));
      ++begin1;
    }
    else
    {
      queue.push(std::move(*begin2));
      ++begin2;
    }
    if (begin1 == end1)
    {
      for (auto i = begin2; i != end2; ++i)
      {
        queue.push(std::move(*i));
      }
      break;
    }
    if (begin2 == end2)
    {
      for (auto i = begin1; i != end1; ++i)
      {
        queue.push(std::move(*i));
      }
      break;
    }
  }
  while (!queue.isEmpty())
  {
    *(first1++) = std::move(queue.getNext());
    queue.drop();
  }
}

template< typename FwdIterator, typename Compare >
FwdIterator ivlicheva::detail::getMax(FwdIterator first, FwdIterator last, Compare cmp)
{
  FwdIterator max = first;
  while (first != last)
  {
    if (cmp(*max, *first))
    {
      max = first;
    }
    ++first;
  }
  return max;
}

template< typename FwdIterator, typename Compare >
FwdIterator ivlicheva::detail::getMin(FwdIterator first, FwdIterator last, Compare cmp)
{
  FwdIterator min = first;
  while (first != last)
  {
    if (cmp(*first, *min))
    {
      min = first;
    }
    ++first;
  }
  return min;
}

namespace ivlicheva
{
  template< typename BidIterator, typename Compare >
  void timsort(BidIterator, const size_t&, Compare);
  template< typename FwdIterator, typename Compare >
  void odd_even(FwdIterator, FwdIterator, Compare);
  template< typename FwdIterator, typename Compare >
  void bucket(FwdIterator, FwdIterator, Compare);
}

template< typename BidIterator, typename Compare >
void ivlicheva::timsort(BidIterator first, const size_t& size, Compare cmp)
{
  struct Minrun
  {
    static size_t getMinrun(size_t n)
    {
      static size_t r = 0;
      while (n >= 64)
      {
        r |= n & 1;
        n >>= 1;
      }
      return n + r;
    }
  };
  size_t minrun = Minrun::getMinrun(size);
  Dictionary< size_t, BidIterator, std::less< size_t > > dictOfIters;
  BidIterator beginOfRun = first;
  BidIterator endOfRun = first;
  size_t tmpSize = size, begin = 0, end = 1;
  while (tmpSize > 0)
  {
    if (endOfRun != first)
    {
      beginOfRun = endOfRun;
    }    
    size_t run = 1;
    --tmpSize;
    while (tmpSize > 0 && cmp(*endOfRun, *(++endOfRun)))
    {
      ++run;
      --tmpSize;
    }
    ++run;
    ++endOfRun;
    --tmpSize;
    if (run < minrun)
    {
      while (tmpSize > 0 && run != minrun)
      {
        ++run;
        ++endOfRun;
        --tmpSize;
      }
    }
    detail::insertionsort(beginOfRun, endOfRun, cmp);
    dictOfIters.push(begin, beginOfRun);
    begin += 2;
    dictOfIters.push(end, endOfRun);
    end += 2;
  }
  size_t n = 1;
  while (n < end - 2)
  {
    detail::mergesort(dictOfIters.get(0), dictOfIters.get(n), dictOfIters.get(n + 1), dictOfIters.get(n + 2), cmp);
    n += 2;
  }
}

template< typename FwdIterator, typename Compare >
void ivlicheva::odd_even(FwdIterator first, FwdIterator last, Compare cmp)
{
  if (std::distance(first, last) <= 1)
  {
    return;
  }
  size_t n = std::distance(first, last);
  bool isSorted = 0;
  while (!isSorted)
  {
    isSorted = 1;
    FwdIterator tmp = first;
    for (size_t i = 0; i < n - 1; ++i)
    {
      if (i % 2 == 0)
      {
        FwdIterator even = tmp;
        FwdIterator odd = ++tmp;
        if (cmp(*odd, *even))
        {
          std::iter_swap(even, odd);
          isSorted = 0;
        }
      }
      else
      {
        FwdIterator odd = tmp;
        FwdIterator even = ++tmp;
        if (cmp(*even, *odd))
        {
          std::iter_swap(odd, even);
          isSorted = 0;
        }
      }
    }
  }
}
#include <iostream>
template< typename FwdIterator, typename Compare >
void ivlicheva::bucket(FwdIterator first, FwdIterator last, Compare cmp)
{
  if (first == last)
  {
    return;
  }
  FwdIterator tmp = first;
  if (++tmp == last)
  {
    return;
  }
  using value_type = typename FwdIterator::value_type;
  struct Border
  {
    static value_type leftBorder(const value_type& min, const value_type& max, size_t n, size_t index)
    {
      if (index == 0)
      {
        return min - 1;
      }
      return rightBorder(min, max, n, index - 1);
    }
    static value_type rightBorder(const value_type& min, const value_type& max, size_t n, size_t index)
    {
      return std::ceil(1.f * (max - min) / n) * (index + 1) + min + 1;
    }
  };
  FwdIterator max = detail::getMax(first, last, cmp);
  FwdIterator min = detail::getMin(first, last, cmp);
  size_t n = 1;
  value_type with2bucket = Border::rightBorder(*min, *max, 2, 0);
  if (cmp(*min, with2bucket) && cmp(with2bucket, *max))
  {
    n = 2;
  }
  Dictionary< size_t, BidirectionalList< value_type >, std::less< size_t > > dictOfBuckets;
  for (size_t i = 0; i < n; ++i)
  {
    BidirectionalList< value_type > list;
    dictOfBuckets.push(i, list);
  }
  FwdIterator tmpFirst = first;
  while (tmpFirst != last)
  {
    for (size_t i = 0; i < n; ++i)
    {
      if (cmp(*tmpFirst, Border::rightBorder(*min, *max, n, i)) && !cmp(*tmpFirst, Border::leftBorder(*min, *max, n, i)))
      {
        dictOfBuckets.get(i).pushBack(*tmpFirst);
        break;
      }
    }
    ++tmpFirst;
  }
  for (size_t i = 0; i < n; ++i)
  {
    if (!dictOfBuckets.get(i).isEmpty())
    {
      typename BidirectionalList< value_type >::Iterator begin = dictOfBuckets.get(i).begin();
      typename BidirectionalList< value_type >::Iterator end = dictOfBuckets.get(i).end();
      odd_even(begin, end, cmp);
    }
  }
  for (size_t i = 0; i < n; ++i)
  {
    for (auto&& j: dictOfBuckets.get(i))
    {
      *first = j;
      ++first;
    }
  }
}

#endif
