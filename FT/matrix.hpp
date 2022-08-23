#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <cstddef>
#include <vector>
#include <iostream>
#include <cmath>
#include <iterator>
#include <algorithm>

namespace ivlicheva
{
  template< typename T >
  class Matrix
  {
    public:
      Matrix() = delete;
      Matrix(const Matrix< T >&) = default;
      Matrix(Matrix< T >&&) noexcept = default;
      Matrix(const size_t&, const size_t&);
      Matrix(const size_t&, const size_t&, const std::vector< T >&);
      Matrix(std::istream&);
      ~Matrix() = default;

      Matrix< T >& operator=(const Matrix< T >&) = default;
      Matrix< T >& operator=(Matrix< T >&&) noexcept = default;
      Matrix< T > operator+(const Matrix< T >&);
      Matrix< T > operator-(const Matrix< T >&);
      Matrix< T > operator*(const Matrix< T >&);
      Matrix< T >& operator+=(const Matrix< T >&);
      Matrix< T >& operator-=(const Matrix< T >&);
      Matrix< T >& operator*=(const Matrix< T >&);

      const size_t& getRows() const noexcept;
      const size_t& getColumns() const noexcept;
      const size_t& getSize() const noexcept;
      const T& getElement(const size_t&, const size_t&) const;
      T& getElement(const size_t&, const size_t&);
      T getDeterminant();
      template< typename F >
      F traverse(F f);
      template< typename F >
      F traverse(F f) const;

    private:
      size_t rows_;
      size_t columns_;
      std::vector< T > vector_;
      size_t getIndex(const size_t&, const size_t&) const;
      ivlicheva::Matrix< T > getNewMatrix(size_t, size_t);
  };
  template< typename T >
  Matrix< T > operator*=(Matrix< T >&, int);
  template< typename T >
  Matrix< T > operator*(const Matrix< T >&, int);
  template< typename T >
  Matrix< T > operator*(int, const Matrix< T >&);
  template< typename T >
  std::istream& operator>>(std::istream&, Matrix< T >&);
  template< typename T >
  std::ostream& operator<<(std::ostream&, Matrix< T >&);
}

template< typename T >
ivlicheva::Matrix< T >::Matrix(const size_t& rows, const size_t& columns):
  rows_(rows),
  columns_(columns),
  vector_()
{
  if (!rows || !columns)
  {
    throw std::logic_error("Bad size");
  }
  vector_.resize(rows_ * columns_);
}

template< typename T >
ivlicheva::Matrix< T >::Matrix(const size_t& rows, const size_t& columns, const std::vector< T >& vector):
  rows_(rows),
  columns_(columns),
  vector_(vector)
{
  if (!rows || !columns || vector.empty())
  {
    throw std::logic_error("Bad matrix");
  }
}

template< typename T >
ivlicheva::Matrix< T >::Matrix(std::istream& in):
  rows_(0),
  columns_(0),
  vector_()
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    throw std::logic_error("Bad stream");
  }
  in >> rows_ >> columns_;
  if (!rows_ || !columns_)
  {
    throw std::logic_error("Bad size");
  }
  std::copy_n(std::istream_iterator< T >(in), rows_ * columns_, std::back_inserter(vector_));
  if (!in)
  {
    throw std::logic_error("Bad stream");
  }
}

template< typename T >
ivlicheva::Matrix< T > ivlicheva::Matrix< T >::operator+(const Matrix< T >& ob)
{
  if (rows_ != ob.rows_ || columns_ != ob.columns_)
  {
    throw std::logic_error("Addition error");
  }
  Matrix< T > tmp(rows_, columns_);
  for (size_t i = 0; i < rows_ * columns_; ++i)
  {
    tmp.vector_[i] = vector_[i] + ob.vector_[i];
  }
  return tmp;
}

template< typename T >
ivlicheva::Matrix< T > ivlicheva::Matrix< T >::operator-(const Matrix< T >& ob)
{
  if (rows_ != ob.rows_ || columns_ != ob.columns_)
  {
    throw std::logic_error("Subtraction error");
  }
  Matrix< T > tmp(rows_, columns_);
  for (size_t i = 0; i < rows_ * columns_; ++i)
  {
    tmp.vector_[i] = vector_[i] - ob.vector_[i];
  }
  return tmp;
}

template< typename T >
ivlicheva::Matrix< T > ivlicheva::Matrix< T >::operator*(const Matrix< T >& ob)
{
  if (columns_ != ob.rows_)
  {
    throw std::logic_error("Division error");
  }
  Matrix< T > tmp(rows_, ob.columns_);
  for (size_t i = 0; i < rows_; ++i)
  {
    for (size_t j = 0; j < ob.columns_; ++j)
    {
      for (size_t k = 0; k < columns_; ++k)
      {
        tmp.vector_[getIndex(i, j)] += (vector_[getIndex(i, k)] * ob.vector_[getIndex(k, j)]);
      }
    }
  }
  return tmp;
}

template< typename T >
size_t ivlicheva::Matrix< T >::getIndex(const size_t& i, const size_t& j) const
{
  return i * columns_ + j;
}

template< typename T >
ivlicheva::Matrix< T >& ivlicheva::Matrix< T >::operator+=(const Matrix< T >& ob)
{
  return *this = *this + ob;
}

template< typename T >
ivlicheva::Matrix< T >& ivlicheva::Matrix< T >::operator-=(const Matrix< T >& ob)
{
  return *this = *this - ob;
}

template< typename T >
ivlicheva::Matrix< T >& ivlicheva::Matrix< T >::operator*=(const Matrix< T >& ob)
{
  return *this = *this * ob;
}

template< typename T >
const size_t& ivlicheva::Matrix< T >::getRows() const noexcept
{
  return rows_;
}

template< typename T >
const size_t& ivlicheva::Matrix< T >::getColumns() const noexcept
{
  return columns_;
}

template< typename T >
const size_t& ivlicheva::Matrix< T >::getSize() const noexcept
{
  return rows_ * columns_;
}

template< typename T >
const T& ivlicheva::Matrix< T >::getElement(const size_t& i, const size_t& j) const
{
  return vector_.at(getIndex(i, j));
}

template< typename T >
T& ivlicheva::Matrix< T >::getElement(const size_t& i, const size_t& j)
{
  return vector_.at(getIndex(i, j));
}

template< typename T >
T ivlicheva::Matrix< T >::getDeterminant()
{
  if (rows_ != columns_)
  {
    throw std::logic_error("Error in calculating the determinant");
  }
  if (rows_ == 1)
  {
    return vector_[0];
  }
  else if (rows_ == 2)
  {
    return vector_[0] * vector_[3] - vector_[1] * vector_[2];
  }
  T det = 0;
  for (size_t i = 1; i < rows_ + 1; ++i)
  {
    det += std::pow(-1, i + 1) * vector_[i - 1] * getNewMatrix(0, i - 1).getDeterminant();
  }
  return det;
}

template< typename T >
ivlicheva::Matrix< T > ivlicheva::Matrix< T >::getNewMatrix(size_t i0, size_t j0)
{
  std::vector< T > vector;
  for (size_t i = 0; i < rows_; ++i)
  {
    for (size_t j = 0; j < columns_; ++j)
    {
      if (i != i0 && j != j0)
      {
        vector.push_back(getElement(i, j));
      }
    }
  }
  return Matrix< T >(rows_ - 1, columns_ - 1, vector);
}

template< typename T >
template< typename F >
F ivlicheva::Matrix< T >::traverse(F f)
{
  for (size_t i = 0; i < rows_; ++i)
  {
    for (size_t j = 0; j < columns_; ++j)
    {
      f(getElement(i, j));
    }
  }
  return f;
}

template< typename T >
template< typename F >
F ivlicheva::Matrix< T >::traverse(F f) const
{
  for (size_t i = 0; i < rows_; ++i)
  {
    for (size_t j = 0; j < columns_; ++j)
    {
      f(getElement(i, j));
    }
  }
  return f;
}

template< typename T >
ivlicheva::Matrix< T > ivlicheva::operator*=(Matrix< T >& ob, int x)
{
  ob.traverse(
   [x](T& value)
   {
     value *= x;
   });
  return ob;
}

template< typename T >
ivlicheva::Matrix< T > ivlicheva::operator*(const Matrix< T >& ob, int x)
{
  Matrix< T > tmp(ob);
  tmp *= x;
  return tmp;
}

template< typename T >
ivlicheva::Matrix< T > ivlicheva::operator*(int x, const Matrix< T >& ob)
{
  return ob * x;
}

template< typename T >
std::istream& ivlicheva::operator>>(std::istream& in, Matrix< T >& ob)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  size_t rows = 0, columns = 0;
  in >> rows;
  in >> columns;
  std::vector< T > vector(rows * columns);
  for (size_t i = 0; i < rows * columns; ++i)
  {
    in >> vector[i];
  }
  if (in)
  {
    ob = Matrix< T >(rows, columns, vector);
  }
  return in;
}

template< typename T >
std::ostream& ivlicheva::operator<<(std::ostream& out, Matrix< T >& ob)
{
  for (size_t i = 0; i < ob.getRows(); ++i)
  {
    for (size_t j = 0; j < ob.getColumns(); ++j)
    {
      out << ob.getElement(i, j) << ' ';
    }
    out << '\n';
  }
  return out;
}

#endif
