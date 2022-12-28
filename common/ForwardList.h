#ifndef FORWARDLIST_H
#define FORWARDLIST_H

#include <cassert>
#include <stdexcept>
#include <iterator>

namespace ivlicheva
{
  template< typename T >
  class ForwardList
  {
    public:
      class Iterator;
      class ConstIterator;

      ForwardList();
      ForwardList(const ForwardList< T >& ob);
      ForwardList(ForwardList< T >&& ob) noexcept;
      ~ForwardList();

      ForwardList< T >& operator= (const ForwardList< T >& ob);
      ForwardList< T >& operator= (ForwardList< T >&& ob) noexcept;

      void swap(ForwardList< T >& ob) noexcept;
      void pushAfter(const T& value, Iterator constIter);
      void pushBack(const T& value);
      void dropAfter(Iterator constIter);
      bool isEmpty() const noexcept;
      const T& getHead() const;
      const T& getTail() const;
      void clear();

      ConstIterator cbeforeBegin() const;
      ConstIterator cbegin() const;
      ConstIterator cend() const;
      ConstIterator begin() const;
      ConstIterator end() const;
      Iterator begin();
      Iterator end();
      Iterator beforeBegin();

    private:
      struct node_t
      {
        T value_;
        node_t* next_;
      };
      node_t* beforeHead_;
  };
}

template< typename T >
class ivlicheva::ForwardList< T >::Iterator: public std::iterator< std::forward_iterator_tag, T >
{
  public:
    friend class ForwardList< T >;
    using this_t = Iterator;
    Iterator();
    Iterator(ConstIterator);
    Iterator(const this_t&) = default;
    ~Iterator() = default;

    this_t& operator=(const this_t&) = default;
    this_t& operator++();
    this_t operator++(int);

    T& operator*();
    T* operator->();
    const T& operator*() const;
    const T* operator->() const;

    bool operator!=(const this_t&) const;
    bool operator==(const this_t&) const;

  private:
    ConstIterator iter_;
};

template< typename T >
ivlicheva::ForwardList< T >::Iterator::Iterator():
  iter_()
{}

template< typename T >
ivlicheva::ForwardList< T >::Iterator::Iterator(ConstIterator iter):
  iter_(iter)
{}

template< typename T >
typename ivlicheva::ForwardList< T >::Iterator& ivlicheva::ForwardList< T >::Iterator::operator++()
{
  ++iter_;
  return *this;
}

template< typename T >
typename ivlicheva::ForwardList< T >::Iterator::this_t ivlicheva::ForwardList< T >::Iterator::operator++(int)
{
  return Iterator(iter_++);
}

template< typename T >
T& ivlicheva::ForwardList< T >::Iterator::operator*()
{
  return const_cast< T& >(*iter_);
}

template< typename T >
T* ivlicheva::ForwardList< T >::Iterator::operator->()
{
  return std::addressof(const_cast< T& >(*iter_));
}

template< typename T >
const T& ivlicheva::ForwardList< T >::Iterator::operator*() const
{
  return *iter_;
}

template< typename T >
const T * ivlicheva::ForwardList< T >::Iterator::operator->() const
{
  return std::addressof(*iter_);
}

template< typename T >
bool ivlicheva::ForwardList< T >::Iterator::operator==(const this_t& rhs) const
{
  return iter_ == rhs.iter_;
}

template< typename T >
bool ivlicheva::ForwardList< T >::Iterator::operator!=(const this_t& rhs) const
{
  return !(rhs == *this);
}

template< typename T >
class ivlicheva::ForwardList< T >::ConstIterator: public std::iterator< std::forward_iterator_tag, T >
{
  public:
    friend class ForwardList< T >;
    using this_t = ConstIterator;

    ConstIterator();
    ConstIterator(const this_t&) = default;
    ~ConstIterator() = default;

    this_t& operator=(const this_t&) = default;
    this_t& operator++();
    this_t operator++(int);

    const T& operator*() const;
    const T* operator->() const;

    bool operator!=(const this_t&) const;
    bool operator==(const this_t&) const;

  private:
    node_t* node_;
    const ForwardList< T >* addressOfList_;
    ConstIterator(node_t*, const ForwardList< T >*);
};

template< typename T >
ivlicheva::ForwardList< T >::ConstIterator::ConstIterator():
  node_(nullptr),
  addressOfList_(nullptr)
{}

template< typename T >
ivlicheva::ForwardList< T >::ConstIterator::ConstIterator(node_t* node, const ForwardList< T >* addressOfList):
  node_(node),
  addressOfList_(addressOfList)
{}

template< typename T >
typename ivlicheva::ForwardList< T >::ConstIterator& ivlicheva::ForwardList< T >::ConstIterator::operator++()
{
  assert(node_ != nullptr);
  node_ = node_->next_;
  return *this;
}

template< typename T >
typename ivlicheva::ForwardList< T >::ConstIterator::this_t ivlicheva::ForwardList< T >::ConstIterator::operator++(int)
{
  assert(node_ != nullptr);
  this_t result(*this);
  ++(*this);
  return result;
}

template< typename T >
const T& ivlicheva::ForwardList< T >::ConstIterator::operator*() const
{
  assert(node_);
  return node_->value_;
}

template< typename T >
const T * ivlicheva::ForwardList< T >::ConstIterator::operator->() const
{
  assert(node_);
  return std::addressof(node_->value_);
}

template< typename T >
bool ivlicheva::ForwardList< T >::ConstIterator::operator==(const this_t& rhs) const
{
  return node_ == rhs.node_ && addressOfList_ == rhs.addressOfList_;
}

template< typename T >
bool ivlicheva::ForwardList< T >::ConstIterator::operator!=(const this_t& rhs) const
{
  return !(rhs == *this);
}

template< typename T >
ivlicheva::ForwardList< T >::ForwardList():
  beforeHead_(static_cast< node_t* >(operator new(sizeof(node_t))))
{
  beforeHead_->next_ = nullptr;
}

template< typename T >
ivlicheva::ForwardList< T >::ForwardList(ForwardList< T >&& ob) noexcept:
  beforeHead_(ob.beforeHead_)
{
  ob.beforeHead_ = nullptr;
}

template< typename T >
ivlicheva::ForwardList< T >::ForwardList(const ForwardList< T >& ob):
  ForwardList()
{
  if (!ob.isEmpty())
  {
    try
    {
      node_t* temp = ob.beforeHead_->next_->next_;
      beforeHead_->next_ = new node_t{ob.beforeHead_->next_->value_, nullptr};
      node_t* tempThis = beforeHead_->next_;
      while (temp)
      {
        tempThis->next_ = new node_t{temp->value_, nullptr};
        tempThis = tempThis->next_;
        temp = temp->next_;
      }
    }
    catch (...)
    {
      clear();
      throw;
    }
  }
}

template< typename T >
ivlicheva::ForwardList< T >::~ForwardList()
{
  clear();
  operator delete(beforeHead_);
  beforeHead_ = nullptr;
}

template< typename T >
ivlicheva::ForwardList< T >& ivlicheva::ForwardList< T >::operator= (const ForwardList< T >& ob)
{
  if (this != std::addressof(ob))
  {
    ForwardList< T > temp(ob);
    swap(temp);
  }
  return *this;
}

template< typename T >
ivlicheva::ForwardList< T >& ivlicheva::ForwardList< T >::operator= (ForwardList< T >&& ob) noexcept
{
  if (this != std::addressof(ob))
  {
    ForwardList< T > tmp(std::move(ob));
    swap(tmp);
  }
  return *this;
}

template< typename T >
void ivlicheva::ForwardList< T >::swap(ForwardList< T >& ob) noexcept
{
  std::swap(beforeHead_, ob.beforeHead_);
}

template< typename T >
void ivlicheva::ForwardList< T >::pushAfter(const T& value, Iterator constIter)
{
  if (constIter == cend())
  {
    throw std::logic_error("Error");
  }
  node_t* temp = constIter.iter_.node_;
  node_t* node = new node_t{value, temp->next_};
  temp->next_ = node;
}

template< typename T >
void ivlicheva::ForwardList< T >::pushBack(const T& value)
{
  node_t* node = new node_t{value, nullptr};
  if (isEmpty())
  {
    beforeHead_->next_ = node;
    return;
  }
  node_t* tmp = beforeHead_->next_;
  while (tmp->next_)
  {
    tmp = tmp->next_;
  }
  tmp->next_ = node;
}

template< typename T >
void ivlicheva::ForwardList< T >::dropAfter(Iterator constIter)
{
  if (constIter == cend())
  {
    throw std::logic_error("Error");
  }
  node_t* temp = constIter.iter_.node_;
  node_t* node = temp->next_;
  temp->next_ = node->next_;
  delete node;
}

template< typename T >
bool ivlicheva::ForwardList< T >::isEmpty() const noexcept
{
  return !beforeHead_->next_;
}

template< typename T >
const T& ivlicheva::ForwardList< T >::getHead() const
{
  if (!beforeHead_->next_)
  {
    throw std::logic_error("ForwardList is empty");
  }
  return beforeHead_->next_->value_;
}

template< typename T >
const T& ivlicheva::ForwardList< T >::getTail() const
{
  if (!beforeHead_->next_)
  {
    throw std::logic_error("ForwardList is empty");
  }
  node_t* temp = beforeHead_->next_;
  while (temp->next_)
  {
    temp = temp->next_;
  }
  return temp->value_;
}

template< typename T >
void ivlicheva::ForwardList< T >::clear()
{
  node_t* head_ = beforeHead_->next_;
  while (head_)
  {
    node_t* temp = head_->next_;
    delete head_;
    head_ = temp;
  }
  beforeHead_->next_ = nullptr;
}

template< typename T >
typename ivlicheva::ForwardList< T >::ConstIterator ivlicheva::ForwardList< T >::cbeforeBegin() const
{
  return ConstIterator(beforeHead_, this);
}

template< typename T >
typename ivlicheva::ForwardList< T >::ConstIterator ivlicheva::ForwardList< T >::cbegin() const
{
  return ConstIterator(beforeHead_->next_, this);
}

template< typename T >
typename ivlicheva::ForwardList< T >::ConstIterator ivlicheva::ForwardList< T >::cend() const
{
  return ConstIterator(nullptr, this);
}

template< typename T >
typename ivlicheva::ForwardList< T >::ConstIterator ivlicheva::ForwardList< T >::begin() const
{
  return ConstIterator(beforeHead_->next_, this);
}

template< typename T >
typename ivlicheva::ForwardList< T >::ConstIterator ivlicheva::ForwardList< T >::end() const
{
  return ConstIterator(nullptr, this);
}

template< typename T >
typename ivlicheva::ForwardList< T >::Iterator ivlicheva::ForwardList< T >::begin()
{
  return ConstIterator(beforeHead_->next_, this);
}

template< typename T >
typename ivlicheva::ForwardList< T >::Iterator ivlicheva::ForwardList< T >::end()
{
  return ConstIterator(nullptr, this);
}

template< typename T >
typename ivlicheva::ForwardList< T >::Iterator ivlicheva::ForwardList< T >::beforeBegin()
{
  return ConstIterator(beforeHead_, this);
}

#endif
