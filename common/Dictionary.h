#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdexcept>
#include <utility>
#include "BinarySearchTree.h"

namespace ivlicheva
{
  template< typename Key, typename Value, typename Compare >
  class Dictionary
  {
    public:
      using data_t = typename std::pair< Key, Value >;
      using iterator_t = typename BinarySearchTree< Key, Value, Compare >::Iterator;
      using citerator_t = typename BinarySearchTree< Key, Value, Compare >::ConstIterator;

      Dictionary() = default;
      Dictionary(const Dictionary&) = default;
      Dictionary(Dictionary&&) noexcept = default;
      Dictionary(std::initializer_list< std::pair< Key, Value > >);
      ~Dictionary() = default;

      Dictionary< Key, Value, Compare >& operator=(const Dictionary< Key, Value, Compare >&) = default;
      Dictionary< Key, Value, Compare >& operator=(Dictionary< Key, Value, Compare >&&) noexcept = default;

      void push(const Key&, const Value&);
      Value& get(const Key&);
      const Value& get(const Key&) const;
      void drop(const Key&);
      iterator_t begin();
      iterator_t end();
      citerator_t cbegin();
      citerator_t cend();
      bool isEmpty() const noexcept;

    private:
      BinarySearchTree< Key, Value, Compare > data_;
      bool isEqual(const Key&, const Key&) const;
      bool isNotEqual(const Key&, const Key&) const;
  };

  template< typename Key, typename Value, typename Compare >
  using dict_iter_t = typename BinarySearchTree< Key, Value, Compare >::Iterator;
  template< typename Key, typename Value, typename Compare >
  using dict_citer_t = typename BinarySearchTree< Key, Value, Compare >::ConstIterator;
}

template< typename Key, typename Value, typename Compare >
ivlicheva::Dictionary< Key, Value, Compare >::Dictionary(std::initializer_list< std::pair< Key, Value > > il):
  data_()
{
  for (auto&& item: il)
  {
    push(item.first, item.second);
  }
}

template< typename Key, typename Value, typename Compare >
void ivlicheva::Dictionary< Key, Value, Compare >::push(const Key& k, const Value& v)
{
  data_.push(k, v);
}

template< typename Key, typename Value, typename Compare >
Value& ivlicheva::Dictionary< Key, Value, Compare >::get(const Key& k)
{
  return const_cast< Value& >(static_cast< const Dictionary< Key, Value, Compare >& >(*this).get(k));
}

template< typename Key, typename Value, typename Compare >
const Value& ivlicheva::Dictionary< Key, Value, Compare >::get(const Key& k) const
{
  typename ivlicheva::BinarySearchTree< Key, Value, Compare >::Iterator iter;
  iter = data_.begin();
  while (iter != data_.end() && isNotEqual(iter->first, k))
  {
    ++iter;
  }
  if (iter == data_.end())
  {
    throw std::logic_error("Error in get");
  }
  return iter->second;
}

template< typename Key, typename Value, typename Compare >
void ivlicheva::Dictionary< Key, Value, Compare >::drop(const Key& k)
{
  data_.drop(k);
}

template< typename Key, typename Value, typename Compare >
typename ivlicheva::dict_iter_t< Key, Value, Compare > ivlicheva::Dictionary< Key, Value, Compare >::begin()
{
  return data_.begin();
}

template< typename Key, typename Value, typename Compare >
typename ivlicheva::dict_iter_t< Key, Value, Compare > ivlicheva::Dictionary< Key, Value, Compare >::end()
{
  return data_.end();
}

template< typename Key, typename Value, typename Compare >
typename ivlicheva::dict_citer_t< Key, Value, Compare > ivlicheva::Dictionary< Key, Value, Compare >::cbegin()
{
  return data_.cbegin();
}

template< typename Key, typename Value, typename Compare >
typename ivlicheva::dict_citer_t< Key, Value, Compare > ivlicheva::Dictionary< Key, Value, Compare >::cend()
{
  return data_.cend();
}

template< typename Key, typename Value, typename Compare >
bool ivlicheva::Dictionary< Key, Value, Compare >::isEmpty() const noexcept
{
  return data_.isEmpty();
}

template< typename Key, typename Value, typename Compare >
bool ivlicheva::Dictionary< Key, Value, Compare >::isEqual(const Key& k1, const Key& k2) const
{
  return !Compare()(k1, k2) && !Compare()(k2, k1);
}

template< typename Key, typename Value, typename Compare >
bool ivlicheva::Dictionary< Key, Value, Compare >::isNotEqual(const Key& k1, const Key& k2) const
{
  return !isEqual(k1, k2);
}

#endif
