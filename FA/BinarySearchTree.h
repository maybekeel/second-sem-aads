#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H

#include <iterator>
#include <stdexcept>
#include <cassert>
#include "Stack.h"
#include "Queue.h"

namespace ivlicheva
{
  template< typename K, typename V, typename C >
  class BinarySearchTree
  {
    public:
      class ConstIterator;
      class Iterator;
      using data_t = std::pair< K, V >;
      struct tree_t
      {
        data_t data_;
        tree_t* parent_;
        tree_t* left_;
        tree_t* right_;
        char color_;
      };
      using this_t = BinarySearchTree< K, V, C >;

      BinarySearchTree();
      BinarySearchTree(const this_t&);
      BinarySearchTree(this_t&&) noexcept;
      BinarySearchTree(std::initializer_list< std::pair< K, V > >);
      ~BinarySearchTree();

      this_t& operator=(const this_t&);
      this_t& operator=(this_t&&) noexcept;
      V& operator[](const K&);
      const V& operator[](const K&) const;

      void swap(this_t&) noexcept;
      void drop(const K&);
      const V& getElement(const K&) const;
      size_t count(const K&);
      Iterator find(const K&);
      template< typename... Args >
      Iterator emplace(const K&, Args&&...);
      Iterator push(const K&, const V&);
      Iterator upperBound(const K&);
      Iterator lowerBound(const K&);
      Iterator erase(Iterator);
      void erase(Iterator, Iterator);
      Iterator insert(const std::pair< K, V >&);
      void insert(std::initializer_list< std::pair< K, V > >);
      ConstIterator upperBound(const K&) const;
      ConstIterator lowerBound(const K&) const;
      bool isEmpty() const noexcept;

      Iterator begin();
      Iterator end();
      ConstIterator begin() const;
      ConstIterator end() const;
      ConstIterator cbegin() const;
      ConstIterator cend() const;

      template< typename F >
      F traverseLNR(F) const;
      template< typename F >
      F traverseRNL(F) const;
      template< typename F >
      F traverseBreadth(F) const;

    private:
      tree_t* root_;
      tree_t* nil_;
      C cmp_;

      void destroy();
      void clear(tree_t*);
      void add(tree_t*, tree_t*, tree_t*);
      void drop(tree_t*);
      void balancePush(tree_t*);
      void balanceDrop(tree_t*);
      void turnSmallLeft(tree_t*);
      void turnSmallRight(tree_t*);
      void turnSmall(tree_t*);
      void turnBigLeft(tree_t*);
      void turnBigRight(tree_t*);
      void turnBig(tree_t*);
      void colorize(tree_t*, char);
      size_t getHigh(tree_t*) const;
      size_t getBlackHigh(tree_t*) const;
      size_t getDepth(tree_t*) const;
      tree_t* getUncle(tree_t*) const;
      tree_t* getNext(tree_t*) const;
      tree_t* getPrev(tree_t*) const;
      tree_t* getMin(tree_t*) const;
      tree_t* getMax(tree_t*) const;
      bool isRight(const tree_t*) const;
      bool isLeft(const tree_t*) const;
      bool isInside(const tree_t*) const;
      bool isNil(const tree_t*) const;
      bool isRed(const tree_t*) const;
      bool isBlack(const tree_t*) const;
      bool isLess(const K&, const K&) const;
      bool isEqual(const K&, const K&) const;
  };
  template< typename K, typename V, typename C >
  using BST = BinarySearchTree< K, V, C >;
}

template< typename K, typename V, typename C >
class ivlicheva::BinarySearchTree< K, V, C >::ConstIterator: public std::iterator< std::forward_iterator_tag, std::pair< K, V > >
{
  public:
    friend class BinarySearchTree< K, V, C >;
    using this_t = ConstIterator;

    ConstIterator();
    ConstIterator(const this_t&) = default;
    ~ConstIterator() = default;

    this_t& operator=(const this_t&) = default;
    this_t& operator++();
    this_t operator++(int);
    this_t& operator--();
    this_t operator--(int);

    const data_t& operator*() const;
    const data_t* operator->() const;

    bool operator!=(const this_t&) const;
    bool operator==(const this_t&) const;

  private:
    tree_t* leaf_;
    const BinarySearchTree< K, V, C >* tree_;
    ConstIterator(tree_t*, const BinarySearchTree< K, V, C >*);
};

template< typename K, typename V, typename C >
ivlicheva::BinarySearchTree< K, V, C >::ConstIterator::ConstIterator():
  leaf_(nullptr),
  tree_(nullptr)
{}

template< typename K, typename V, typename C >
ivlicheva::BinarySearchTree< K, V, C >::ConstIterator::ConstIterator(tree_t* leaf, const BinarySearchTree< K, V, C >* tree):
  leaf_(leaf),
  tree_(tree)
{}

template< typename K, typename V, typename C >
typename ivlicheva::BinarySearchTree< K, V, C >::ConstIterator& ivlicheva::BinarySearchTree< K, V, C >::ConstIterator::operator++()
{
  leaf_ = tree_->getNext(leaf_);
  return *this;
}

template< typename K, typename V, typename C >
typename ivlicheva::BinarySearchTree< K, V, C >::ConstIterator ivlicheva::BinarySearchTree< K, V, C >::ConstIterator::operator++(int)
{
  this_t result(*this);
  ++(*this);
  return result;
}

template< typename K, typename V, typename C >
typename ivlicheva::BinarySearchTree< K, V, C >::ConstIterator& ivlicheva::BinarySearchTree< K, V, C >::ConstIterator::operator--()
{
  leaf_ = tree_->getPrev(leaf_);
  return *this;
}

template< typename K, typename V, typename C >
typename ivlicheva::BinarySearchTree< K, V, C >::ConstIterator ivlicheva::BinarySearchTree< K, V, C >::ConstIterator::operator--(int)
{
  this_t result(*this);
  --(*this);
  return result;
}

template< typename K, typename V, typename C >
const typename ivlicheva::BinarySearchTree< K, V, C >::data_t& ivlicheva::BinarySearchTree< K, V, C >::ConstIterator::operator*() const
{
  return leaf_->data_;
}

template< typename K, typename V, typename C >
const typename ivlicheva::BinarySearchTree< K, V, C >::data_t* ivlicheva::BinarySearchTree< K, V, C >::ConstIterator::operator->() const
{
  return std::addressof(leaf_->data_);
}

template< typename K, typename V, typename C >
bool ivlicheva::BinarySearchTree< K, V, C >::ConstIterator::operator==(const this_t& iter) const
{
  return leaf_ == iter.leaf_ && tree_ == iter.tree_;
}

template< typename K, typename V, typename C >
bool ivlicheva::BinarySearchTree< K, V, C >::ConstIterator::operator!=(const this_t& iter) const
{
  return !(*this == iter);
}

template< typename K, typename V, typename C >
class ivlicheva::BinarySearchTree< K, V, C >::Iterator: public std::iterator< std::forward_iterator_tag, std::pair< K, V > >
{
  public:
    friend class BinarySearchTree< K, V, C >;
    using this_t = Iterator;

    Iterator();
    Iterator(const this_t&) = default;
    Iterator(ConstIterator);
    ~Iterator() = default;

    this_t& operator=(const this_t&) = default;
    this_t& operator++();
    this_t operator++(int);
    this_t& operator--();
    this_t operator--(int);

    data_t& operator*();
    data_t* operator->();
    const data_t& operator*() const;
    const data_t* operator->() const;

    bool operator!=(const this_t&) const;
    bool operator==(const this_t&) const;

  private:
    ConstIterator citer_;
};

template< typename K, typename V, typename C >
ivlicheva::BinarySearchTree< K, V, C >::Iterator::Iterator():
  citer_()
{}

template< typename K, typename V, typename C >
ivlicheva::BinarySearchTree< K, V, C >::Iterator::Iterator(ConstIterator citer):
  citer_(citer)
{}

template< typename K, typename V, typename C >
typename ivlicheva::BinarySearchTree< K, V, C >::Iterator& ivlicheva::BinarySearchTree< K, V, C >::Iterator::operator++()
{
  ++citer_;
  return *this;
}

template< typename K, typename V, typename C >
typename ivlicheva::BinarySearchTree< K, V, C >::Iterator ivlicheva::BinarySearchTree< K, V, C >::Iterator::operator++(int)
{
  this_t result(*this);
  ++(*this);
  return result;
}

template< typename K, typename V, typename C >
typename ivlicheva::BinarySearchTree< K, V, C >::Iterator& ivlicheva::BinarySearchTree< K, V, C >::Iterator::operator--()
{
  --citer_;
  return *this;
}

template< typename K, typename V, typename C >
typename ivlicheva::BinarySearchTree< K, V, C >::Iterator ivlicheva::BinarySearchTree< K, V, C >::Iterator::operator--(int)
{
  this_t result(*this);
  --(*this);
  return result;
}

template< typename K, typename V, typename C >
typename ivlicheva::BinarySearchTree< K, V, C >::data_t& ivlicheva::BinarySearchTree< K, V, C >::Iterator::operator*()
{
  return const_cast< data_t& >(*citer_);
}

template< typename K, typename V, typename C >
typename ivlicheva::BinarySearchTree< K, V, C >::data_t* ivlicheva::BinarySearchTree< K, V, C >::Iterator::operator->()
{
  return std::addressof(const_cast< data_t& >(*citer_));
}

template< typename K, typename V, typename C >
const typename ivlicheva::BinarySearchTree< K, V, C >::data_t& ivlicheva::BinarySearchTree< K, V, C >::Iterator::operator*() const
{
  return *citer_;
}

template< typename K, typename V, typename C >
const typename ivlicheva::BinarySearchTree< K, V, C >::data_t* ivlicheva::BinarySearchTree< K, V, C >::Iterator::operator->() const
{
  return std::addressof(*citer_);
}

template< typename K, typename V, typename C >
bool ivlicheva::BinarySearchTree< K, V, C >::Iterator::operator==(const this_t& iter) const
{
  return citer_ == iter.citer_;
}

template< typename K, typename V, typename C >
bool ivlicheva::BinarySearchTree< K, V, C >::Iterator::operator!=(const this_t& iter) const
{
  return !(*this == iter);
}

template< typename K, typename V, typename C >
typename ivlicheva::BinarySearchTree< K, V, C >::Iterator ivlicheva::BinarySearchTree< K, V, C >::begin()
{
  return cbegin();
}

template< typename K, typename V, typename C >
typename ivlicheva::BinarySearchTree< K, V, C >::Iterator ivlicheva::BinarySearchTree< K, V, C >::end()
{
  return cend();
}

template< typename K, typename V, typename C >
typename ivlicheva::BinarySearchTree< K, V, C >::ConstIterator ivlicheva::BinarySearchTree< K, V, C >::begin() const
{
  return cbegin();
}

template< typename K, typename V, typename C >
typename ivlicheva::BinarySearchTree< K, V, C >::ConstIterator ivlicheva::BinarySearchTree< K, V, C >::end() const
{
  return cend();
}

template< typename K, typename V, typename C >
typename ivlicheva::BinarySearchTree< K, V, C >::ConstIterator ivlicheva::BinarySearchTree< K, V, C >::cbegin() const
{
  return ConstIterator(getMin(root_), this);
}

template< typename K, typename V, typename C >
typename ivlicheva::BinarySearchTree< K, V, C >::ConstIterator ivlicheva::BinarySearchTree< K, V, C >::cend() const
{
  return ConstIterator(nil_, this);
}

template< typename K, typename V, typename C >
ivlicheva::BinarySearchTree< K, V, C >::BinarySearchTree():
  root_(nullptr),
  nil_(static_cast< tree_t* >(operator new(sizeof(tree_t))))
{
  colorize(nil_, 'b');
}

template< typename K, typename V, typename C >
ivlicheva::BinarySearchTree< K, V, C >::BinarySearchTree(const this_t& ob):
  BinarySearchTree()
{
  if (!ob.isEmpty())
  {
    tree_t* obLeaf = ob.root_;
    try
    {
      root_ = new tree_t{obLeaf->data_, nullptr, nil_, nil_, obLeaf->color_};
      add(root_, obLeaf, ob.nil_);
    }
    catch (...)
    {
      destroy();
      throw;
    }
  }
}

template< typename K, typename V, typename C >
ivlicheva::BinarySearchTree< K, V, C >::BinarySearchTree(this_t&& ob) noexcept:
  root_(ob.root_),
  nil_(ob.nil_)
{
  ob.root_ = nullptr;
  ob.nil_ = nullptr;
}

template< typename K, typename V, typename C >
ivlicheva::BinarySearchTree< K, V, C >::BinarySearchTree(std::initializer_list< std::pair< K, V > > il):
  BinarySearchTree()
{
  this->insert(il);
}

template< typename K, typename V, typename C >
ivlicheva::BinarySearchTree< K, V, C >::~BinarySearchTree()
{
  destroy();
}

template< typename K, typename V, typename C >
void ivlicheva::BinarySearchTree< K, V, C >::destroy()
{
  if (root_)
  {
    clear(root_);
    delete root_;
  }
  operator delete(nil_);
}

template< typename K, typename V, typename C >
void ivlicheva::BinarySearchTree< K, V, C >::clear(tree_t* leaf)
{
  if (leaf && !isNil(leaf))
  {
    clear(leaf->left_);
    clear(leaf->right_);
    if (!isNil(leaf->left_))
    {
      delete leaf->left_;
    }
    if (!isNil(leaf->right_))
    {
      delete leaf->right_;
    }
  }
}

template< typename K, typename V, typename C >
void ivlicheva::BinarySearchTree< K, V, C >::add(tree_t* newLeaf, tree_t* obLeaf, tree_t* obNil)
{
  if (obLeaf->left_ != obNil)
  {
    newLeaf->left_ = new tree_t{obLeaf->left_->data_, newLeaf, nil_, nil_};
    add(newLeaf->left_, obLeaf->left_, obNil);
  }
  if (obLeaf->right_ != obNil)
  {
    newLeaf->right_ = new tree_t{obLeaf->right_->data_, newLeaf, nil_, nil_};
    add(newLeaf->right_, obLeaf->right_, obNil);
  }
}

template< typename K, typename V, typename C >
size_t ivlicheva::BinarySearchTree< K, V, C >::count(const K& key)
{
  auto iter = begin();
  while (iter != end())
  {
    if (isEqual(iter->first, key))
    {
      return 1;
    }
    ++iter;
  }
  return 0;
}

template< typename K, typename V, typename C >
typename ivlicheva::BinarySearchTree< K, V, C >::Iterator ivlicheva::BinarySearchTree< K, V, C >::find(const K& key)
{
  auto iter = begin();
  while (iter != end())
  {
    if (isEqual(iter->first, key))
    {
      break;
    }
    ++iter;
  }
  return iter;
}

template< typename K, typename V, typename C >
ivlicheva::BST< K, V, C >& ivlicheva::BinarySearchTree< K, V, C >::operator=(const this_t& ob)
{
  if (this != std::addressof(ob))
  {
    this_t temp(ob);
    swap(temp);
  }
  return *this;
}

template< typename K, typename V, typename C >
ivlicheva::BST< K, V, C >& ivlicheva::BinarySearchTree< K, V, C >::operator=(this_t&& ob) noexcept
{
  if (this != std::addressof(ob))
  {
    this_t tmp(std::move(ob));
    swap(tmp);
  }
  return *this;
}

template< typename K, typename V, typename C >
V& ivlicheva::BinarySearchTree< K, V, C >::operator[](const K& key)
{
  return const_cast< V& >(getElement(key));
}

template< typename K, typename V, typename C >
const V& ivlicheva::BinarySearchTree< K, V, C >::operator[](const K& key) const
{
  return getElement(key);
}

template< typename K, typename V, typename C >
void ivlicheva::BinarySearchTree< K, V, C >::swap(this_t& ob) noexcept
{
  std::swap(root_, ob.root_);
  std::swap(nil_, ob.nil_);
}

template< typename K, typename V, typename C >
const V& ivlicheva::BinarySearchTree< K, V, C >::getElement(const K& key) const
{
  auto iter = begin();
  while (iter != end())
  {
    if (isEqual(iter->first, key))
    {
      break;
    }
    ++iter;
  }
  return iter->second;
}

template< typename K, typename V, typename C >
void ivlicheva::BinarySearchTree< K, V, C >::balancePush(tree_t* leaf)
{
  if (leaf == root_)
  {
    return;
  }
  if (isRed(leaf->parent_))
  {
    if (leaf->parent_ == root_)
    {
      colorize(leaf->parent_, 'b');
      return;
    }
    tree_t* uncle = getUncle(leaf);
    tree_t* parent = leaf->parent_;
    tree_t* grandParent = leaf->parent_->parent_;
    if (uncle && isRed(uncle))
    {
      colorize(uncle, 'b');
      colorize(parent, 'b');
      colorize(grandParent, 'r');
      balancePush(grandParent);
    }
    else if (uncle && isBlack(uncle))
    {
      if (isInside(leaf) && isRight(leaf))
      {
        turnSmallLeft(leaf);
        balancePush(leaf);
      }
      else if (isInside(leaf) && isLeft(leaf))
      {
        turnSmallRight(leaf);
        balancePush(leaf);
      }
      else
      {
        colorize(parent, 'b');
        colorize(grandParent, 'r');
        if (isRight(parent))
        {
          turnSmallLeft(parent);
        }
        else
        {
          turnSmallRight(parent);
        }
      }
    }
    else
    {
      colorize(parent, 'b');
      colorize(grandParent, 'r');
      if (isRight(parent))
      {
        turnSmallLeft(parent);
      }
      else
      {
        turnSmallRight(parent);
      }
    }
  }
  else
  {
    tree_t* tmp = leaf->parent_;
    if (getBlackHigh(tmp->left_) > getBlackHigh(tmp->right_))
    {
      while (getBlackHigh(tmp->left_) != getBlackHigh(tmp->right_))
      {
        tmp = tmp->right_;
        if (isRed(tmp))
        {
          colorize(tmp, 'b');
        }
      }
      return;
    }
    if (getBlackHigh(tmp->left_) < getBlackHigh(tmp->right_))
    {
      while (getBlackHigh(tmp->left_) != getBlackHigh(tmp->right_))
      {
        tmp = tmp->left_;
        if (isRed(tmp))
        {
          colorize(tmp, 'b');
        }
      }
    }
  }
}

template< typename K, typename V, typename C >
void ivlicheva::BinarySearchTree< K, V, C >::turnBigLeft(tree_t* leaf)
{
  turnSmallRight(leaf);
  turnSmallLeft(leaf);
}

template< typename K, typename V, typename C >
void ivlicheva::BinarySearchTree< K, V, C >::turnBigRight(tree_t* leaf)
{
  turnSmallLeft(leaf);
  turnSmallRight(leaf);
}

template< typename K, typename V, typename C >
void ivlicheva::BinarySearchTree< K, V, C >::turnSmallLeft(tree_t* leaf)
{
  if (!isNil(leaf->left_))
  {
    leaf->parent_->right_ = leaf->left_;
    leaf->left_->parent_ = leaf->parent_;
  }
  else
  {
    leaf->parent_->right_ = nil_;
  }
  leaf->left_ = leaf->parent_;
  if (leaf->parent_->parent_)
  {
    tree_t* tmp = leaf->parent_->parent_;
    leaf->parent_->parent_ = leaf;
    leaf->parent_ = tmp;
    if (isLess(leaf->data_.first, tmp->data_.first))
    {
      tmp->left_ = leaf;
    }
    else
    {
      tmp->right_ = leaf;
    }
  }
  else
  {
    leaf->parent_->parent_ = leaf;
    leaf->parent_ = nullptr;
    root_ = leaf;
  }
}

template< typename K, typename V, typename C >
void ivlicheva::BinarySearchTree< K, V, C >::turnSmallRight(tree_t* leaf)
{
  if (!isNil(leaf->right_))
  {
    leaf->parent_->left_ = leaf->right_;
    leaf->right_->parent_ = leaf->parent_;
  }
  else
  {
    leaf->parent_->left_ = nil_;
  }
  leaf->right_ = leaf->parent_;
  if (leaf->parent_->parent_)
  {
    tree_t* tmp = leaf->parent_->parent_;
    leaf->parent_->parent_ = leaf;
    leaf->parent_ = tmp;
    if (isLess(leaf->data_.first, tmp->data_.first))
    {
      tmp->left_ = leaf;
    }
    else
    {
      tmp->right_ = leaf;
    }
  }
  else
  {
    leaf->parent_->parent_ = leaf;
    leaf->parent_ = nullptr;
    root_ = leaf;
  }
}

template< typename K, typename V, typename C >
size_t ivlicheva::BinarySearchTree< K, V, C >::getDepth(tree_t* leaf) const
{
  size_t depth = 0;
  while (leaf->parent_)
  {
    leaf = leaf->parent_;
    ++depth;
  }
  return depth;
}

template< typename K, typename V, typename C >
typename ivlicheva::BinarySearchTree< K, V, C >::tree_t* ivlicheva::BinarySearchTree< K, V, C >::getUncle(tree_t* leaf) const
{
  if (isLess(leaf->parent_->data_.first, leaf->parent_->parent_->data_.first))
  {
    return leaf->parent_->parent_->right_;
  }
  return leaf->parent_->parent_->left_;
}

template< typename K, typename V, typename C >
typename ivlicheva::BinarySearchTree< K, V, C >::tree_t* ivlicheva::BinarySearchTree< K, V, C >::getNext(tree_t* leaf) const
{
  if (!leaf || isNil(leaf))
  {
    throw std::logic_error("Bad leaf");
  }
  if (isNil(leaf->left_) && isNil(leaf->right_))
  {
    if (!leaf->parent_)
    {
      return nil_;
    }
    if (isLeft(leaf))
    {
      return leaf->parent_;
    }
    if (isRight(leaf))
    {
      if (leaf == getMax(root_))
      {
        return nil_;
      }
      if (leaf->parent_->parent_)
      {
        return leaf->parent_->parent_;
      }
      return nil_;
    }
  }
  if (!isNil(leaf->right_))
  {
    return getMin(leaf->right_);
  }
  if (isNil(leaf->right_))
  {
    if (leaf == getMax(root_))
    {
      return nil_;
    }
    return leaf->parent_;
  }
  return nil_;
}

template< typename K, typename V, typename C >
typename ivlicheva::BinarySearchTree< K, V, C >::tree_t* ivlicheva::BinarySearchTree< K, V, C >::getPrev(tree_t* leaf) const
{
  if (!leaf || isNil(leaf))
  {
    throw std::logic_error("Bad leaf");
  }
  if (isNil(leaf->left_) && isNil(leaf->right_))
  {
    if (!leaf->parent_)
    {
      return nil_;
    }
    if (isLeft(leaf))
    {
      if (leaf == getMin(root_))
      {
        return nil_;
      }
      if (leaf == getMin(root_->right_))
      {
        return root_;
      }
      return leaf->parent_->parent_;
    }
    if (isRight(leaf))
    {
      return leaf->parent_;
    }
  }
  if (!isNil(leaf->left_))
  {
    return getMax(leaf->left_);
  }
  if (isNil(leaf->left_))
  {
    if (leaf == getMin(root_->right_))
    {
      return root_;
    }
    return leaf->parent_;
  }
  return nil_;
}

template< typename K, typename V, typename C >
bool ivlicheva::BinarySearchTree< K, V, C >::isRight(const tree_t* leaf) const
{
  if (!leaf->parent_)
  {
    throw std::logic_error("Error");
  }
  return leaf == leaf->parent_->right_;
}

template< typename K, typename V, typename C >
bool ivlicheva::BinarySearchTree< K, V, C >::isLeft(const tree_t* leaf) const
{
  return !isRight(leaf);
}

template< typename K, typename V, typename C >
bool ivlicheva::BinarySearchTree< K, V, C >::isInside(const tree_t* leaf) const
{
  if (isLess(leaf->parent_->data_.first, leaf->parent_->parent_->data_.first))
  {
    return !isLess(leaf->data_.first, leaf->parent_->data_.first);
  }
  return isLess(leaf->data_.first, leaf->parent_->data_.first);
}

template< typename K, typename V, typename C >
size_t ivlicheva::BinarySearchTree< K, V, C >::getBlackHigh(tree_t* leaf) const
{
  if (!leaf || isNil(leaf))
  {
    return 0;
  }
  return std::max(getBlackHigh(leaf->left_), getBlackHigh(leaf->right_)) + (leaf->color_ == 'b' ? 1 : 0);
}

template< typename K, typename V, typename C >
size_t ivlicheva::BinarySearchTree< K, V, C >::getHigh(tree_t* leaf) const
{
  if (isNil(leaf))
  {
    return 0;
  }
  return std::max(getHigh(leaf->left_), getHigh(leaf->right_)) + 1;
}

template< typename K, typename V, typename C >
typename ivlicheva::BinarySearchTree< K, V, C >::Iterator ivlicheva::BinarySearchTree< K, V, C >::push(const K& k, const V& v)
{
  tree_t* leaf = new tree_t{{k, v}, nullptr, nil_, nil_, 'r'};
  if (!root_)
  {
    root_ = leaf;
    return ConstIterator(leaf, this);
  }
  tree_t* tmp = root_;
  while (!isNil(tmp))
  {
    if (isLess(k, tmp->data_.first))
    {
      if (isNil(tmp->left_))
      {
        leaf->parent_ = tmp;
        tmp->left_ = leaf;
        balancePush(leaf);
        break;
      }
      tmp = tmp->left_;
    }
    if (!isLess(k, tmp->data_.first))
    {
      if (isNil(tmp->right_))
      {
        leaf->parent_ = tmp;
        tmp->right_ = leaf;
        balancePush(leaf);
        break;
      }
      tmp = tmp->right_;
    }
  }
  return ConstIterator(leaf, this);
}

template< typename K, typename V, typename C >
template< typename... Args >
typename ivlicheva::BinarySearchTree< K, V, C >::Iterator ivlicheva::BinarySearchTree< K, V, C >::emplace(const K& k, Args&&... args)
{
  return push(k, V(std::forward< Args >(args)...));
}

template< typename K, typename V, typename C >
typename ivlicheva::BinarySearchTree< K, V, C >::Iterator ivlicheva::BinarySearchTree< K, V, C >::upperBound(const K& k)
{
  return const_cast< const this_t& >(*this).upperBound(k);
}

template< typename K, typename V, typename C >
typename ivlicheva::BinarySearchTree< K, V, C >::Iterator ivlicheva::BinarySearchTree< K, V, C >::lowerBound(const K& k)
{
  return const_cast< const this_t& >(*this).lowerBound(k);
}

template< typename K, typename V, typename C >
typename ivlicheva::BinarySearchTree< K, V, C >::ConstIterator ivlicheva::BinarySearchTree< K, V, C >::upperBound(const K& k) const
{
  ConstIterator iter = begin();
  while (iter != end() && isLess(iter.leaf_->data_.first, k))
  {
    ++iter;
  }
  return iter;
}

template< typename K, typename V, typename C >
typename ivlicheva::BinarySearchTree< K, V, C >::ConstIterator ivlicheva::BinarySearchTree< K, V, C >::lowerBound(const K& k) const
{
  ConstIterator iter = begin();
  while (iter != end() && !isLess(k, iter.leaf_->data_.first))
  {
    ++iter;
  }
  return iter;
}

template< typename K, typename V, typename C >
typename ivlicheva::BinarySearchTree< K, V, C >::Iterator ivlicheva::BinarySearchTree< K, V, C >::erase(Iterator iter)
{
  if (iter == end())
  {
    return end();
  }
  Iterator iter2 = iter;
  ++iter2;
  drop(iter->first);
  return iter2;
}

template< typename K, typename V, typename C >
void ivlicheva::BinarySearchTree< K, V, C >::erase(Iterator first, Iterator last)
{
  while (first != last)
  {
    first = erase(first);
  }
}

template< typename K, typename V, typename C >
typename ivlicheva::BinarySearchTree< K, V, C >::Iterator ivlicheva::BinarySearchTree< K, V, C >::insert(const std::pair< K, V >& p)
{
  return push(p.first, p.second);
}

template< typename K, typename V, typename C >
void ivlicheva::BinarySearchTree< K, V, C >::insert(std::initializer_list< std::pair< K, V > > il)
{
  for (auto&& item: il)
  {
    insert(item);
  }
}

template< typename K, typename V, typename C >
void ivlicheva::BinarySearchTree< K, V, C >::drop(const K& k)
{
  if (isEqual(k, root_->data_.first))
  {
    drop(root_);
    return;
  }
  tree_t* tmp = root_;
  while (tmp)
  {
    if (isLess(k, tmp->data_.first))
    {
      tmp = tmp->left_;
      if (tmp->data_.first == k)
      {
        drop(tmp);
        break;
      }
    }
    if (!isLess(k, tmp->data_.first))
    {
      tmp = tmp->right_;
      if (tmp->data_.first == k)
      {
        drop(tmp);
        break;
      }
    }
  }
}

template< typename K, typename V, typename C >
void ivlicheva::BinarySearchTree< K, V, C >::balanceDrop(tree_t* leaf)
{
  if (isRed(leaf))
  {
    if (getBlackHigh(leaf->left_) > getBlackHigh(leaf->right_) && isBlack(leaf->left_))
    {
      if (isBlack(leaf->left_->left_) && isBlack(leaf->left_->right_))
      {
        std::swap(leaf->color_, leaf->left_->color_);
      }
      else if (isRed(leaf->left_->left_))
      {
        std::swap(leaf->color_, leaf->left_->color_);
        colorize(leaf->left_->left_, 'b');
        turnSmallRight(leaf->left_);
      }
    }
    else if (getBlackHigh(leaf->left_) < getBlackHigh(leaf->right_) && isBlack(leaf->right_))
    {
      if (isBlack(leaf->right_->left_) && isBlack(leaf->right_->right_))
      {
        std::swap(leaf->color_, leaf->right_->color_);
      }
      else if (isRed(leaf->right_->right_))
      {
        std::swap(leaf->color_, leaf->right_->color_);
        colorize(leaf->right_->right_, 'b');
        turnSmallLeft(leaf->right_);
      }
    }
  }
  else if (isBlack(leaf))
  {
    if (getBlackHigh(leaf->left_) > getBlackHigh(leaf->right_))
    {
      if (isRed(leaf->left_))
      {
        if (isBlack(leaf->left_->right_->left_) && isBlack(leaf->left_->right_->right_))
        {
          std::swap(leaf->left_->color_, leaf->left_->right_->color_);
          turnSmallRight(leaf->left_);
        }
        else if (isRed(leaf->left_->right_->left_))
        {
          colorize(leaf->left_->right_->left_, 'b');
          turnBigRight(leaf->left_->right_);
        }
      }
      else
      {
        if (isRed(leaf->left_->right_))
        {
          colorize(leaf->left_->right_, 'b');
          turnBigRight(leaf->left_->right_);
        }
        else if (isBlack(leaf->left_->right_) && isBlack(leaf->left_->left_))
        {
          colorize(leaf->left_, 'r');
          if (leaf->parent_)
          {
            balanceDrop(leaf->parent_);
          }
        }
      }
    }
    else if (getBlackHigh(leaf->left_) < getBlackHigh(leaf->right_))
    {
      if (isRed(leaf->right_))
      {
        if (isBlack(leaf->right_->left_->left_) && isBlack(leaf->right_->left_->right_))
        {
          std::swap(leaf->right_->color_, leaf->right_->left_->color_);
          turnSmallLeft(leaf->right_);
        }
        else if (isRed(leaf->right_->left_->right_))
        {
          colorize(leaf->right_->left_->right_, 'b');
          turnBigLeft(leaf->right_->left_);
        }
        else
        {
          colorize(leaf->left_->left_, 'b');
          turnSmallRight(leaf->left_);
        }
      }
      else
      {
        if (isRed(leaf->right_->left_))
        {
          colorize(leaf->right_->left_, 'b');
          turnBigLeft(leaf->right_->left_);
        }
        else if (isBlack(leaf->right_->right_) && isBlack(leaf->right_->left_))
        {
          colorize(leaf->right_, 'r');
          if (leaf->parent_)
          {
            balanceDrop(leaf->parent_);
          }
        }
        else
        {
          colorize(leaf->right_->right_, 'b');
          turnSmallLeft(leaf->right_);
        }
      }
    }
  }
}

template< typename K, typename V, typename C >
void ivlicheva::BinarySearchTree< K, V, C >::drop(tree_t* leaf)
{
  if (isRed(leaf))
  {
    if (isNil(leaf->left_) && isNil(leaf->right_))
    {
      if (isLeft(leaf))
      {
        leaf->parent_->left_ = nil_;
      }
      else
      {
        leaf->parent_->right_ = nil_;
      }
      delete leaf;
    }
    else if (!isNil(leaf->left_) && !isNil(leaf->right_))
    {
      if (getHigh(leaf->left_) > getHigh(leaf->right_))
      {
        tree_t* maxLeft = getMax(leaf->left_);
        std::swap(leaf->data_, maxLeft->data_);
        drop(maxLeft);
      }
      else
      {
        tree_t* minRight = getMin(leaf->right_);
        std::swap(leaf->data_, minRight->data_);
        drop(minRight);
      }
    }
  }
  else if (isBlack(leaf))
  {
    if (!isNil(leaf->left_) && isNil(leaf->right_))
    {
      std::swap(leaf->data_, leaf->left_->data_);
      drop(leaf->left_);
    }
    else if (isNil(leaf->left_) && !isNil(leaf->right_))
    {
      std::swap(leaf->data_, leaf->right_->data_);
      drop(leaf->right_);
    }
    else if (!isNil(leaf->left_) && !isNil(leaf->right_))
    {
      if (getHigh(leaf->left_) > getHigh(leaf->right_))
      {
        tree_t* maxLeft = getMax(leaf->left_);
        std::swap(leaf->data_, maxLeft->data_);
        drop(maxLeft);
      }
      else
      {
        tree_t* minRight = getMin(leaf->right_);
        std::swap(leaf->data_, minRight->data_);
        drop(minRight);
      }
    }
    else if (isNil(leaf->left_) && isNil(leaf->right_))
    {
      if (isLeft(leaf))
      {
        leaf->parent_->left_ = nil_;
      }
      else
      {
        leaf->parent_->right_ = nil_;
      }
      balanceDrop(leaf->parent_);
      delete leaf;
    }
  }
}

template < typename K, typename V, typename C >
typename ivlicheva::BinarySearchTree< K, V, C >::tree_t* ivlicheva::BinarySearchTree< K, V, C >::getMax(tree_t* leaf) const
{
  if (!leaf)
  {
    return nil_;
  }
  while (!isNil(leaf->right_))
  {
    leaf = leaf->right_;
  }
  return leaf;
}

template < typename K, typename V, typename C >
typename ivlicheva::BinarySearchTree< K, V, C >::tree_t* ivlicheva::BinarySearchTree< K, V, C >::getMin(tree_t* leaf) const
{
  if (!leaf)
  {
    return nil_;
  }
  while (!isNil(leaf->left_))
  {
    leaf = leaf->left_;
  }
  return leaf;
}

template< typename K, typename V, typename C >
bool ivlicheva::BinarySearchTree< K, V, C >::isNil(const tree_t* leaf) const
{
  return leaf == nil_;
}

template< typename K, typename V, typename C >
bool ivlicheva::BinarySearchTree< K, V, C >::isRed(const tree_t* leaf) const
{
  return leaf->color_ == 'r';
}

template< typename K, typename V, typename C >
bool ivlicheva::BinarySearchTree< K, V, C >::isBlack(const tree_t* leaf) const
{
  return leaf->color_ == 'b';
}

template < typename K, typename V, typename C >
bool ivlicheva::BinarySearchTree< K, V, C >::isEmpty() const noexcept
{
  return !root_;
}

template< typename K, typename V, typename C >
template< typename F >
F ivlicheva::BinarySearchTree< K, V, C >::traverseLNR(F f) const
{
  if (!root_)
  {
    return f;
  }
  tree_t* tmp = root_;
  if (isNil(tmp->right_) && !isNil(tmp->left_))
  {
    f(tmp->left_->data_);
    f(tmp->data_);
    return f;
  }
  Stack< tree_t* > stack;
  stack.push(tmp);
  while (tmp != getMax(root_))
  {
    if (!isNil(tmp->left_))
    {
      tmp = tmp->left_;
      stack.push(tmp);
    }
    else
    {
      f(tmp->data_);
      stack.drop();
      if (!stack.isEmpty())
      {
        tmp = stack.getTop();
        f(tmp->data_);
        stack.drop();
      }
      if (!isNil(tmp->right_))
      {
        tmp = tmp->right_;
        stack.push(tmp);
      }
      else
      {
        tmp = stack.getTop();
        f(tmp->data_);
        stack.drop();
        tmp = tmp->right_;
        stack.push(tmp);
      }
    }
  }
  f(stack.getTop()->data_);
  stack.drop();
  return f;
}

template< typename K, typename V, typename C >
template< typename F >
F ivlicheva::BinarySearchTree< K, V, C >::traverseRNL(F f) const
{
  if (!root_)
  {
    return f;
  }
  tree_t* tmp = root_;
  if (isNil(tmp->left_) && !isNil(tmp->right_))
  {
    f(tmp->right_->data_);
    f(tmp->data_);
    return f;
  }
  Stack< tree_t* > stack;
  stack.push(tmp);
  while (tmp != getMin(root_))
  {
    if (!isNil(tmp->right_))
    {
      tmp = tmp->right_;
      stack.push(tmp);
    }
    else
    {
      f(tmp->data_);
      stack.drop();
      if (!stack.isEmpty())
      {
        tmp = stack.getTop();
        f(tmp->data_);
        stack.drop();
      }
      if (!isNil(tmp->left_))
      {
        tmp = tmp->left_;
        stack.push(tmp);
      }
      else
      {
        tmp = stack.getTop();
        f(tmp->data_);
        stack.drop();
        tmp = tmp->left_;
        stack.push(tmp);
      }
    }
  }
  f(stack.getTop()->data_);
  stack.drop();
  return f;
}

template< typename K, typename V, typename C >
template< typename F >
F ivlicheva::BinarySearchTree< K, V, C >::traverseBreadth(F f) const
{
  if (!root_)
  {
    return f;
  }
  Queue< tree_t* > queue;
  tree_t* tmp = root_;
  queue.push(tmp);
  while (!queue.isEmpty())
  {
    if (!isNil(tmp->left_))
    {
      queue.push(tmp->left_);
    }
    if (!isNil(tmp->right_))
    {
      queue.push(tmp->right_);
    }
    f(queue.getNext()->data_);
    queue.drop();
    if (!queue.isEmpty())
    {
      tmp = queue.getNext();
    }
  }
  return f;
}

template< typename K, typename V, typename C >
void ivlicheva::BinarySearchTree< K, V, C >::colorize(tree_t* leaf, char c)
{
  assert(c == 'b' || c == 'r');
  leaf->color_ = c;
}

template< typename K, typename V, typename C >
bool ivlicheva::BinarySearchTree< K, V, C >::isLess(const K& k1, const K& k2) const
{
  return cmp_(k1, k2);
}

template< typename K, typename V, typename C >
bool ivlicheva::BinarySearchTree< K, V, C >::isEqual(const K& k1, const K& k2) const
{
  return !isLess(k1, k2) && !isLess(k2, k1);
}

#endif
