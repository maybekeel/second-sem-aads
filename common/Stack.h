#ifndef STACK_H
#define STACK_H
#include "Array.h"

namespace ivlicheva
{
  template< typename T >
  class Stack
  {
    public:
      Stack() = default;
      Stack(const Stack< T >& ob) = default;
      Stack(Stack< T >&& ob) = default;
      Stack(const T& value);
      ~Stack() = default;

      Stack< T >& operator= (const Stack< T >& ob) = default;
      Stack< T >& operator= (Stack< T >&& ob) noexcept = default;

      void push(const T& value);
      void drop();
      const T& getTop() const;
      bool isEmpty() const;

    private:
      detail::Array< T > data_;
  };
}

template< typename T >
ivlicheva::Stack< T >::Stack(const T& value):
  data_(detail::Array< T >(value))
{}

template< typename T >
void ivlicheva::Stack< T >::push(const T& value)
{
  data_.pushBack(value);
}

template< typename T >
void ivlicheva::Stack< T >::drop()
{
  data_.dropBack();
}

template< typename T >
const T& ivlicheva::Stack< T >::getTop() const
{
  return data_.getTop();
}

template< typename T >
bool ivlicheva::Stack< T >::isEmpty() const
{
  return data_.isEmpty();
}

#endif
