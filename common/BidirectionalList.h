#ifndef BIDIRECTIONALLIST_H
#define BIDIRECTIONALLIST_H

#include <cassert>
#include <iterator>

namespace ivlicheva
{
  template< typename T >
  class BidirectionalList
  {
    public:
      class Iterator;
      class ConstIterator;

      BidirectionalList();
      BidirectionalList(const BidirectionalList< T >&);
      BidirectionalList(BidirectionalList< T >&&) noexcept;
      BidirectionalList(std::initializer_list< T >);
      ~BidirectionalList();

      BidirectionalList< T >& operator=(const BidirectionalList< T >&);
      BidirectionalList< T >& operator=(BidirectionalList< T >&&) noexcept;
      bool operator==(BidirectionalList< T >&);
      bool operator!=(BidirectionalList< T >&);

      void swap(BidirectionalList< T >&) noexcept;
      void pushAfter(const T&, Iterator);
      void dropAfter(Iterator);
      void pushBack(const T&);
      template < typename... Args >
      Iterator emplaceFront(Args&&...);
      template < typename... Args >
      Iterator emplaceBack(Args&&...);
      template< typename... Args >
      Iterator emplaceAfter(Iterator, Args&&...);
      Iterator erase(Iterator);
      bool isEmpty() const noexcept;
      const T& getHead() const;
      const T& getTail() const;

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
        node_t* previous_;
      };
      node_t* beforeHead_;
      node_t* tail_;
      void clear();
  };
}

template< typename T >
class ivlicheva::BidirectionalList< T >::Iterator: public std::iterator< std::forward_iterator_tag, T >
{
  public:
    friend class BidirectionalList< T >;
    using this_t = Iterator;
    Iterator();
    Iterator(ConstIterator);
    Iterator(const this_t&) = default;
    ~Iterator() = default;

    this_t& operator=(const this_t&) = default;
    this_t& operator++();
    this_t operator++(int);
    this_t& operator--();
    this_t operator--(int);

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
ivlicheva::BidirectionalList< T >::Iterator::Iterator():
  iter_()
{}

template< typename T >
ivlicheva::BidirectionalList< T >::Iterator::Iterator(ConstIterator iter):
  iter_(iter)
{}

template< typename T >
typename ivlicheva::BidirectionalList< T >::Iterator& ivlicheva::BidirectionalList< T >::Iterator::operator++()
{
  ++iter_;
  return *this;
}

template< typename T >
typename ivlicheva::BidirectionalList< T >::Iterator::this_t ivlicheva::BidirectionalList< T >::Iterator::operator++(int)
{
  return Iterator(iter_++);
}

template< typename T >
typename ivlicheva::BidirectionalList< T >::Iterator::this_t& ivlicheva::BidirectionalList< T >::Iterator::operator--()
{
  --iter_;
  return *this;
}

template< typename T >
typename ivlicheva::BidirectionalList< T >::Iterator::this_t ivlicheva::BidirectionalList< T >::Iterator::operator--(int)
{
  return Iterator(iter_--);
}

template< typename T >
T& ivlicheva::BidirectionalList< T >::Iterator::operator*()
{
  return const_cast< T& >(*iter_);
}

template< typename T >
T* ivlicheva::BidirectionalList< T >::Iterator::operator->()
{
  return std::addressof(const_cast< T& >(*iter_));
}

template< typename T >
const T& ivlicheva::BidirectionalList< T >::Iterator::operator*() const
{
  return *iter_;
}

template< typename T >
const T * ivlicheva::BidirectionalList< T >::Iterator::operator->() const
{
  return std::addressof(*iter_);
}

template< typename T >
bool ivlicheva::BidirectionalList< T >::Iterator::operator==(const this_t& rhs) const
{
  return iter_ == rhs.iter_;
}

template< typename T >
bool ivlicheva::BidirectionalList< T >::Iterator::operator!=(const this_t& rhs) const
{
  return !(rhs == *this);
}

template< typename T >
class ivlicheva::BidirectionalList< T >::ConstIterator: public std::iterator< std::forward_iterator_tag, T >
{
  public:
    friend class BidirectionalList< T >;
    using this_t = ConstIterator;

    ConstIterator();
    ConstIterator(const this_t&) = default;
    ~ConstIterator() = default;

    this_t& operator=(const this_t&) = default;
    this_t& operator++();
    this_t operator++(int);
    this_t& operator--();
    this_t operator--(int);

    const T& operator*() const;
    const T* operator->() const;

    bool operator!=(const this_t&) const;
    bool operator==(const this_t&) const;

  private:
    node_t* node_;
    const BidirectionalList< T >* addressOfList_;
    ConstIterator(node_t*, const BidirectionalList< T >*);
};

template< typename T >
ivlicheva::BidirectionalList< T >::ConstIterator::ConstIterator():
  node_(nullptr),
  addressOfList_(nullptr)
{}

template< typename T >
ivlicheva::BidirectionalList< T >::ConstIterator::ConstIterator(node_t* node, const BidirectionalList< T >* addressOfList):
  node_(node),
  addressOfList_(addressOfList)
{}

template< typename T >
typename ivlicheva::BidirectionalList< T >::ConstIterator& ivlicheva::BidirectionalList< T >::ConstIterator::operator++()
{
  assert(node_ != nullptr);
  node_ = node_->next_;
  return *this;
}

template< typename T >
typename ivlicheva::BidirectionalList< T >::ConstIterator::this_t ivlicheva::BidirectionalList< T >::ConstIterator::operator++(int)
{
  assert(node_ != nullptr);
  this_t result(*this);
  ++(*this);
  return result;
}

template< typename T >
typename ivlicheva::BidirectionalList< T >::ConstIterator::this_t& ivlicheva::BidirectionalList< T >::ConstIterator::operator--()
{
  assert(node_ != nullptr);
  node_ = node_->previous_;
  return *this;
}

template< typename T >
typename ivlicheva::BidirectionalList< T >::ConstIterator::this_t ivlicheva::BidirectionalList< T >::ConstIterator::operator--(int)
{
  assert(node_ != nullptr);
  this_t result(*this);
  --(*this);
  return result;
}

template< typename T >
const T& ivlicheva::BidirectionalList< T >::ConstIterator::operator*() const
{
  assert(node_);
  return node_->value_;
}

template< typename T >
const T * ivlicheva::BidirectionalList< T >::ConstIterator::operator->() const
{
  assert(node_);
  return std::addressof(node_->value_);
}

template< typename T >
bool ivlicheva::BidirectionalList< T >::ConstIterator::operator==(const this_t& rhs) const
{
  return node_ == rhs.node_ && addressOfList_ == rhs.addressOfList_;
}

template< typename T >
bool ivlicheva::BidirectionalList< T >::ConstIterator::operator!=(const this_t& rhs) const
{
  return !(rhs == *this);
}

template< typename T >
ivlicheva::BidirectionalList< T >::BidirectionalList():
  beforeHead_(static_cast< node_t* >(operator new(sizeof(node_t)))),
  tail_(nullptr)
{
  beforeHead_->next_ = nullptr;
}

template< typename T >
ivlicheva::BidirectionalList< T >::BidirectionalList(BidirectionalList< T >&& ob) noexcept:
  beforeHead_(ob.beforeHead_),
  tail_(ob.tail_)
{
  ob.beforeHead_ = nullptr;
  ob.tail_ = nullptr;
}

template< typename T >
ivlicheva::BidirectionalList< T >::BidirectionalList(const BidirectionalList< T >& ob):
  BidirectionalList()
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
      tail_ = tempThis;
    }
    catch (...)
    {
      clear();
      throw;
    }
  }
}

template< typename T >
ivlicheva::BidirectionalList< T >::BidirectionalList(std::initializer_list< T > il):
  beforeHead_()
{
  Iterator iter = beforeBegin();
  for (auto&& item: il)
  {
    pushAfter(item.first, iter);
    ++iter;
  }
}

template< typename T >
ivlicheva::BidirectionalList< T >::~BidirectionalList()
{
  clear();
  operator delete(beforeHead_);
  beforeHead_ = nullptr;
  tail_ = nullptr;
}

template< typename T >
ivlicheva::BidirectionalList< T >& ivlicheva::BidirectionalList< T >::operator=(const BidirectionalList< T >& ob)
{
  if (this != std::addressof(ob))
  {
    BidirectionalList< T > temp(ob);
    swap(temp);
  }
  return *this;
}

template< typename T >
ivlicheva::BidirectionalList< T >& ivlicheva::BidirectionalList< T >::operator=(BidirectionalList< T >&& ob) noexcept
{
  if (this != std::addressof(ob))
  {
    BidirectionalList< T > tmp(std::move(ob));
    swap(tmp);
  }
  return *this;
}

template< typename T >
bool ivlicheva::BidirectionalList< T >::operator==(BidirectionalList< T >& ob)
{
  Iterator iter = begin();
  Iterator iterOb = ob.begin();
  while (iter != end())
  {
    if (*iter != *iterOb)
    {
      return false;
    }
    ++iter;
    ++iterOb;
  }
  return true;
}

template< typename T >
bool ivlicheva::BidirectionalList< T >::operator!=(BidirectionalList< T >& ob)
{
  return !(*this == ob);
}

template< typename T >
void ivlicheva::BidirectionalList< T >::swap(BidirectionalList< T >& ob) noexcept
{
  std::swap(tail_, ob.tail_);
  std::swap(beforeHead_, ob.beforeHead_);
}

template< typename T >
void ivlicheva::BidirectionalList< T >::pushAfter(const T& value, Iterator iter)
{
  if (iter == end())
  {
    throw std::logic_error("end of list");
  }
  iter.iter_.node_->next_ = new node_t{value, iter.iter_.node_->next_, iter.iter_.node_};
}

template< typename T >
void ivlicheva::BidirectionalList< T >::dropAfter(Iterator iter)
{
  if (iter == end())
  {
    throw std::logic_error("end of list");
  }
  if (iter == beforeBegin())
  {
    node_t* tmp = beforeHead_->next_;
    if (tmp)
    {
      beforeHead_->next_ = tmp->next_;
      if (tmp->next_)
      {
        tmp->next_->previous_ = beforeHead_;
      }
      delete tmp;
    }
    return;
  }
  node_t* tmp = iter.iter_.node_->next_;
  if (tmp == tail_)
  {
    tail_ = tmp->previous_;
    iter.iter_.node_->next_ = tmp->next_;
  }
  else
  {
    iter.iter_.node_->next_ = tmp->next_;
    if (tmp->next_)
    {
      tmp->next_->previous_ = iter.iter_.node_;
    }
  }
  delete tmp;
}

template< typename T >
void ivlicheva::BidirectionalList< T >::pushBack(const T& value)
{
  if (isEmpty())
  {
    node_t* node = new node_t{value, nullptr, beforeHead_};
    beforeHead_->next_ = node;
    tail_ = node;
  }
  else
  {
    node_t* node = new node_t{value, nullptr, tail_};
    tail_->next_ = node;
    tail_ = node;
  }
}

template< typename T >
template< typename... Args >
typename ivlicheva::BidirectionalList< T >::Iterator ivlicheva::BidirectionalList< T >::emplaceFront(Args&&... args)
{
  Iterator iter = beforeBegin();
  pushAfter(T(std::forward< Args >(args)...), iter);
  return begin();
}

template< typename T >
template< typename... Args >
typename ivlicheva::BidirectionalList< T >::Iterator ivlicheva::BidirectionalList< T >::emplaceBack(Args&&... args)
{
  Iterator iter = ConstIterator(tail_, this);
  pushAfter(T(std::forward< Args >(args)...), iter);
  return ++iter;
}

template< typename T >
template< typename... Args >
typename ivlicheva::BidirectionalList< T >::Iterator ivlicheva::BidirectionalList< T >::emplaceAfter(Iterator iter, Args&&... args)
{
  pushAfter(T(std::forward< Args >(args)...), iter);
  return iter;
}

template< typename T >
typename ivlicheva::BidirectionalList< T >::Iterator ivlicheva::BidirectionalList< T >::erase(Iterator iter)
{
  if (iter == beforeBegin() || iter == end())
  {
    throw std::logic_error("Bad iter");
  }
  dropAfter(--iter);
  return iter;
}

template< typename T >
bool ivlicheva::BidirectionalList< T >::isEmpty() const noexcept
{
  return !beforeHead_->next_;
}

template< typename T >
const T& ivlicheva::BidirectionalList< T >::getHead() const
{
  if (!beforeHead_->next_)
  {
    throw std::logic_error("BiderectionalList is empty");
  }
  return beforeHead_->next_->value_;
}

template< typename T >
const T& ivlicheva::BidirectionalList< T >::getTail() const
{
  if (!tail_)
  {
    throw std::logic_error("BidirectionalList is empty");
  }
  return tail_->value_;
}

template< typename T >
void ivlicheva::BidirectionalList< T >::clear()
{
  node_t* head_ = beforeHead_->next_;
  while (head_)
  {
    node_t* temp = head_->next_;
    delete head_;
    head_ = temp;
  }
  beforeHead_->next_ = nullptr;
  tail_ = nullptr;
}

template< typename T >
typename ivlicheva::BidirectionalList< T >::ConstIterator ivlicheva::BidirectionalList< T >::cbeforeBegin() const
{
  return ConstIterator(beforeHead_, this);
}

template< typename T >
typename ivlicheva::BidirectionalList< T >::ConstIterator ivlicheva::BidirectionalList< T >::cbegin() const
{
  return ConstIterator(beforeHead_->next_, this);
}

template< typename T >
typename ivlicheva::BidirectionalList< T >::ConstIterator ivlicheva::BidirectionalList< T >::cend() const
{
  return ConstIterator(nullptr, this);
}

template< typename T >
typename ivlicheva::BidirectionalList< T >::ConstIterator ivlicheva::BidirectionalList< T >::begin() const
{
  return ConstIterator(beforeHead_->next_, this);
}

template< typename T >
typename ivlicheva::BidirectionalList< T >::ConstIterator ivlicheva::BidirectionalList< T >::end() const
{
  return ConstIterator(nullptr, this);
}

template< typename T >
typename ivlicheva::BidirectionalList< T >::Iterator ivlicheva::BidirectionalList< T >::begin()
{
  return ConstIterator(beforeHead_->next_, this);
}

template< typename T >
typename ivlicheva::BidirectionalList< T >::Iterator ivlicheva::BidirectionalList< T >::end()
{
  return ConstIterator(nullptr, this);
}

template< typename T >
typename ivlicheva::BidirectionalList< T >::Iterator ivlicheva::BidirectionalList< T >::beforeBegin()
{
  return ConstIterator(beforeHead_, this);
}

#endif
