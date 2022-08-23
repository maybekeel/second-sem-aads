#ifndef QUEUE_H
#define QUEUE_H
#include "Array.h"

namespace ivlicheva
{
  template< typename T >
  class Queue
  {
    public:
      Queue() = default;
      Queue(const Queue< T >& ob) = default;
      Queue(Queue< T >&& ob) noexcept = default;
      Queue(const T& value);
      ~Queue() = default;

      Queue< T >& operator= (const Queue< T >& ob) = default;
      Queue< T >& operator= (Queue< T >&& ob) noexcept = default;

      void push(const T& value);
      void drop();
      const T& getNext() const;
      bool isEmpty() const;

    private:
      detail::Array< T > data_;
  };
}

template< typename T >
ivlicheva::Queue< T >::Queue(const T& value):
  data_(detail::Array< T >(value))
{}

template< typename T >
void ivlicheva::Queue< T >::push(const T& value)
{
  data_.pushBack(value);
}

template< typename T >
void ivlicheva::Queue< T >::drop()
{
  data_.dropFront();
}

template< typename T >
const T& ivlicheva::Queue< T >::getNext() const
{
  return data_.getNext();
}

template< typename T >
bool ivlicheva::Queue< T >::isEmpty() const
{
  return data_.isEmpty();
}

#endif
