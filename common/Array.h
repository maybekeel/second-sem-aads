#ifndef ARRAY_H
#define ARRAY_H

#include <stdexcept>
#include <cassert>

namespace ivlicheva
{
  namespace detail
  {
    template< typename T >
    T* allocateMemory(size_t size)
    {
      return static_cast< T* >(operator new(sizeof(T) * size));
    }

    template< class T1, class T2 >
    void construct(T1* p, const T2& value)
    {
      new(p) T1(value);
    }

    template< class T >
    T* newCopy(const T* src, size_t srcsize, size_t destsize)
    {
      assert(destsize >= srcsize);
      T* dest = allocateMemory< T >(destsize);
      size_t i = 0;
      try
      {
        for (i = 0; i < srcsize; ++i)
        {
          construct(dest + i, src[i]);
        }
      }
      catch (...)
      {
        for (size_t j = 0; j < i; ++j)
        {
          dest[j].~T();
        }
        operator delete(dest);
        throw;
      }
      return dest;
    }
  }
}

namespace ivlicheva
{
  namespace detail
  {
    template< typename T >
    class Array
    {
      public:
        Array();
        Array(const Array< T >& ob);
        Array(Array< T >&& ob) noexcept;
        ~Array();

        Array< T >& operator= (const Array< T >& ob);
        Array< T >& operator= (Array< T >&& ob) noexcept;

        void pushBack(const T& value);
        void dropFront();
        void dropBack();
        void swap(Array< T >& ob) noexcept;
        const T& getNext() const;
        const T& getTop() const;
        bool isEmpty() const;

      private:
        size_t size_;
        size_t used_;
        size_t head_;
        size_t tail_;
        T* array_;
    };
  }
}

template< typename T >
ivlicheva::detail::Array< T >::Array():
  size_(1),
  used_(0),
  head_(0),
  tail_(0),
  array_(allocateMemory< T >(size_))
{}

template< typename T >
ivlicheva::detail::Array< T >::Array(const Array< T >& ob):
  size_(ob.size_),
  used_(0),
  head_(ob.head_),
  tail_(ob.tail_),
  array_(allocateMemory< T >(size_))
{
  try
  {
    for (used_ = 0; used_ < ob.used_; ++used_)
    {
      construct(array_ + used_, ob.array_[used_]);
    }
  }
  catch (...)
  {
    for (size_t i = 0; i < used_; ++i)
    {
      array_[i].~T();
    }
    operator delete(array_);
  }
}

template< typename T >
ivlicheva::detail::Array< T >::Array(Array< T >&& ob) noexcept:
  size_(ob.size_),
  used_(ob.used_),
  head_(ob.head_),
  tail_(ob.tail_),
  array_(ob.array_)
{
  ob.array_ = nullptr;
}

template< typename T >
ivlicheva::detail::Array< T >::~Array()
{
  for (size_t i = head_; i < used_; ++i)
  {
    array_[i % size_].~T();
  }
  operator delete(array_);
}

template< typename T >
ivlicheva::detail::Array< T >& ivlicheva::detail::Array< T >::operator= (const Array< T >& ob)
{
  if (this != std::addressof(ob))
  {
    Array< T > temp(ob);
    swap(temp);
  }
  return *this;
}

template< typename T >
ivlicheva::detail::Array< T >& ivlicheva::detail::Array< T >::operator= (Array< T >&& ob) noexcept
{
  if (this != std::addressof(ob))
  {
    swap(ob);
  }
  return *this;
}

template< typename T >
void ivlicheva::detail::Array< T >::pushBack(const T& value)
{
  if (used_ > size_ - 1)
  {
    T* newArray = newCopy(array_, used_, ++size_);
    try
    {
      construct(newArray + used_, value);
    }
    catch (...)
    {
      for (size_t i = 0; i < used_ + 1; i++)
      {
        newArray[i].~T();
      }
      operator delete(newArray);
      throw;
    }
    for (size_t i = 0; i < used_; i++)
    {
      array_[i].~T();
    }
    operator delete(array_);
    array_ = newArray;
    tail_ = used_;
    ++used_;
  }
  else
  {
    size_t newTail = 0;
    if (used_ == 0)
    {
      newTail = 0;
    }
    else
    {
      newTail = (tail_ + 1) % size_;
    }
    try
    {
      construct(array_ + newTail, value);
    }
    catch (...)
    {
      array_[newTail].~T();
      throw;
    }
    tail_ = newTail;
    ++used_;
  }
}

template< typename T >
void ivlicheva::detail::Array< T >::dropFront()
{
  if (!used_)
  {
    throw std::logic_error("It is empty");
  }
  --used_;
  array_[head_].~T();
  if (head_ != tail_)
  {
    head_ = (head_ + 1) % size_;
  }
  else
  {
    head_ = 0;
    tail_ = 0;
  }
}

template< typename T >
void ivlicheva::detail::Array< T >::dropBack()
{
  if (!used_)
  {
    throw std::logic_error("It is empty");
  }
  --used_;
  array_[tail_].~T();
  if (tail_ != head_)
  {
    tail_ = (tail_ - 1) % size_;
  }
  else
  {
    tail_ = 0;
    head_ = 0;
  }
}

template< typename T >
void ivlicheva::detail::Array< T >::swap(Array< T >& ob) noexcept
{
  std::swap(size_, ob.size_);
  std::swap(used_, ob.used_);
  std::swap(array_, ob.array_);
  std::swap(head_, ob.head_);
  std::swap(tail_, ob.tail_);
}

template< typename T >
const T& ivlicheva::detail::Array< T >::getNext() const
{
  if (isEmpty())
  {
    throw std::logic_error("It is empty");
  }
  return array_[head_];
}

template< typename T >
const T& ivlicheva::detail::Array< T >::getTop() const
{
  if (isEmpty())
  {
    throw std::logic_error("It is empty");
  }
  return array_[tail_];
}

template< typename T >
bool ivlicheva::detail::Array< T >::isEmpty() const
{
  return !used_;
}

#endif
