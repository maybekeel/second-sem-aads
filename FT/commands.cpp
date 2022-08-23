#include "commands.hpp"
#include <iostream>
#include <string>
#include <random>
#include <functional>
#include <iterator>
#include <algorithm>

namespace
{
  bool isNumber(std::string s)
  {
    bool isNumber = true;
    for (size_t i = 0; i < s.size() && isNumber; i++)
    {
      isNumber = isNumber && std::isdigit(s.at(i));
    }
    return isNumber;
  }

  std::string getSubstring(std::string& str)
  {
    if (str.empty())
    {
      return str;
    }
    size_t n = str.find_first_of(' ');
    if (n == 0)
    {
      str.erase(0, 1);
      return getSubstring(str);
    }
    std::string word = str.substr(0, n);
    n = (n == str.npos) ? n : n + 1;
    str.erase(0, n);
    return word;
  }
}

ivlicheva::Commands::Commands(const map_t& matrixMap, std::istream& in, std::ostream& out):
  matrixMap_(matrixMap),
  in_(in),
  out_(out)
{}

bool ivlicheva::Commands::create()
{
  std::string matrixName = "";
  in_ >> matrixName;
  size_t n = 0, m = 0;
  in_ >> n >> m;
  if (!in_)
  {
    in_.clear();
    return false;
  }
  insert(matrixName, Matrix< int >(n, m));
  return true;
}

bool ivlicheva::Commands::input()
{
  std::string matrixName = "";
  in_ >> matrixName;
  insert(matrixName, Matrix< int >(in_));
  return true;
}

bool ivlicheva::Commands::fill()
{
  std::string matrixName = "";
  in_ >> matrixName;
  auto iter = matrixMap_.find(matrixName);
  if (iter == matrixMap_.end())
  {
    return false;
  }
  std::string parameter = "";
  in_ >> parameter;
  if (parameter == "random")
  {
    fillRandom(iter->second);
  }
  else if (isNumber(parameter))
  {
    int number = std::stoi(parameter);
    fillNumber(iter->second, number);
  }
  else
  {
    return false;
  }
  return true;
}

bool ivlicheva::Commands::mult()
{
  std::string newMatrixName = "", matrixName = "", parameter = "";
  in_ >> newMatrixName >> matrixName >> parameter;
  auto iter = matrixMap_.find(matrixName);
  if (iter == matrixMap_.end())
  {
    return false;
  }
  Matrix< int > matrix = iter->second;
  if (isNumber(parameter))
  {
    insert(newMatrixName, matrix * std::stoi(parameter));
  }
  else if (matrixMap_.find(parameter) != matrixMap_.end())
  {
    insert(newMatrixName, matrix * matrixMap_.find(parameter)->second);
  }
  else
  {
    return false;
  }
  return true;
}

bool ivlicheva::Commands::sum()
{
  std::string newMatrixName = "", matrixName1 = "", matrixName2 = "";
  in_ >> newMatrixName >> matrixName1 >> matrixName2;
  auto iter1 = matrixMap_.find(matrixName1);
  auto iter2 = matrixMap_.find(matrixName2);
  if (iter1 == matrixMap_.end() || iter2 == matrixMap_.end())
  {
    return false;
  }
  insert(newMatrixName, iter1->second + iter2->second);
  return true;
}

bool ivlicheva::Commands::substract()
{
  std::string newMatrixName = "", matrixName1 = "", matrixName2 = "";
  in_ >> newMatrixName >> matrixName1 >> matrixName2;
  auto iter1 = matrixMap_.find(matrixName1);
  auto iter2 = matrixMap_.find(matrixName2);
  if (iter1 == matrixMap_.end() || iter2 == matrixMap_.end())
  {
    return false;
  }
  insert(newMatrixName, iter1->second - iter2->second);
  return true;
}

bool ivlicheva::Commands::getDeterminant()
{
  std::string matrixname = "";
  in_ >> matrixname;
  auto iter = matrixMap_.find(matrixname);
  if (iter == matrixMap_.end())
  {
    return false;
  }
  out_ << iter->second.getDeterminant() << '\n';
  return true;
}

bool ivlicheva::Commands::hconcat()
{
  std::string str = "";
  std::getline(in_, str);
  std::string newMatrixName = getSubstring(str);
  std::vector< Matrix< int > > matrices;
  while (!str.empty())
  {
    std::string matrixName = getSubstring(str);
    auto iter = matrixMap_.find(matrixName);
    if (iter == matrixMap_.end())
    {
      return false;
    }
    matrices.push_back(iter->second);
  }
  Matrix< int > result = std::accumulate(std::next(matrices.begin()), matrices.end(), matrices.at(0),
   [&](const Matrix< int >& first, const Matrix< int >& second) -> Matrix< int >
   {
     return dohconcat(first, second);
   });
  insert(newMatrixName, result);
  return true;
}

bool ivlicheva::Commands::vconcat()
{
  std::string str = "";
  std::getline(in_, str);
  std::string newMatrixName = getSubstring(str);
  std::vector< Matrix< int > > matrices;
  while (!str.empty())
  {
    std::string matrixName = getSubstring(str);
    auto iter = matrixMap_.find(matrixName);
    if (iter == matrixMap_.end())
    {
      return false;
    }
    matrices.push_back(iter->second);
  }
  Matrix< int > result = std::accumulate(std::next(matrices.begin()), matrices.end(), matrices.at(0),
   [&](const Matrix< int >& first, const Matrix< int >& second) -> Matrix< int >
   {
     return dovconcat(first, second);
   });
  insert(newMatrixName, result);
  return true;
}

bool ivlicheva::Commands::hrepeat()
{
  std::string newMatrixname = "", matrixName = "";
  in_ >> newMatrixname >> matrixName;
  auto iter = matrixMap_.find(matrixName);
  if (iter == matrixMap_.end())
  {
    return false;
  }
  size_t n = 0;
  in_ >> n;
  if (n == 0)
  {
    return false;
  }
  Matrix< int > tmp(iter->second);
  for (size_t i = 0; i < n - 1; ++i)
  {
    tmp = dohconcat(tmp, iter->second);
  }
  insert(newMatrixname, tmp);
  return true;
}

bool ivlicheva::Commands::vrepeat()
{
  std::string newMatrixname = "", matrixName = "";
  in_ >> newMatrixname >> matrixName;
  auto iter = matrixMap_.find(matrixName);
  if (iter == matrixMap_.end())
  {
    return false;
  }
  size_t n = 0;
  in_ >> n;
  if (n == 0)
  {
    return false;
  }
  Matrix< int > tmp(iter->second);
  for (size_t i = 0; i < n - 1; ++i)
  {
    tmp = dovconcat(tmp, iter->second);
  }
  insert(newMatrixname, tmp);
  return true;
}

bool ivlicheva::Commands::repeat()
{
  std::string newMatrixname = "", matrixName = "";
  in_ >> newMatrixname >> matrixName;
  auto iter = matrixMap_.find(matrixName);
  if (iter == matrixMap_.end())
  {
    return false;
  }
  size_t n = 0, m = 0;
  in_ >> n >> m;
  if (!n || !m)
  {
    return false;
  }
  Matrix< int > tmp(iter->second);
  for (size_t i = 0; i < n - 1; ++i)
  {
    tmp = dohconcat(tmp, iter->second);
  }
  Matrix< int > tmp2(tmp);
  for (size_t i = 0; i < m - 1; ++i)
  {
    tmp = dovconcat(tmp, tmp2);
  }
  insert(newMatrixname, tmp);
  return true;
}

bool ivlicheva::Commands::print()
{
  std::string matrixName = "";
  in_ >> matrixName;
  auto iter = matrixMap_.find(matrixName);
  if (iter == matrixMap_.end())
  {
    return false;
  }
  out_ << iter->second;
  return true;
}

void ivlicheva::Commands::fillRandom(Matrix< int >& matrix)
{
  matrix.traverse(
   [](int& n)
   {
     n = std::rand();
   });
}

void ivlicheva::Commands::fillNumber(Matrix< int >& matrix, const int& number)
{
  matrix.traverse(
   [number](int& n)
   {
     n = number;
   });
}

ivlicheva::Matrix< int > ivlicheva::Commands::dohconcat(const Matrix< int >& matrix1, const Matrix< int >& matrix2)
{
  if (matrix1.getRows() != matrix2.getRows())
  {
    throw std::logic_error("Different rows");
  }
  std::vector< int > vector;
  for (size_t i = 0; i < matrix1.getRows(); ++i)
  {
    for (size_t j = 0; j < matrix1.getColumns(); ++j)
    {
      vector.push_back(matrix1.getElement(i, j));
    }
    for (size_t j = 0; j < matrix2.getColumns(); ++j)
    {
      vector.push_back(matrix2.getElement(i, j));
    }
  }
  return Matrix< int >(matrix1.getRows(), matrix1.getColumns() + matrix2.getColumns(), vector);
}

ivlicheva::Matrix< int > ivlicheva::Commands::dovconcat(const Matrix< int >& matrix1, const Matrix< int >& matrix2)
{
  if (matrix1.getColumns() != matrix2.getColumns())
  {
    throw std::logic_error("Different columns");
  }
  std::vector< int > vector;
  matrix1.traverse(
   [&vector](const int& n)
   {
    vector.push_back(n);
   });
  matrix2.traverse(
   [&vector](const int& n)
   {
    vector.push_back(n);
   });
  return Matrix< int >(matrix1.getRows() + matrix2.getRows(), matrix1.getColumns(), vector);
}

void ivlicheva::Commands::insert(const std::string& name, const Matrix< int >& matrix)
{
  if (matrixMap_.count(name))
  {
    matrixMap_.find(name)->second = matrix;
  }
  else
  {
    matrixMap_.insert({name, matrix});
  }
}
