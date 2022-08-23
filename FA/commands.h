#ifndef COMMANDS_H
#define COMMANDS_H

#include <iosfwd>
#include "matrix.h"

namespace ivlicheva
{
  struct Commands
  {
    using tree_t = BinarySearchTree< std::string, Matrix< int >, std::less< std::string > >;
    public:
      Commands(const tree_t&, std::istream&, std::ostream&);
      bool create();
      bool input();
      bool fill();
      bool mult();
      bool sum();
      bool substract();
      bool getDeterminant();
      bool hconcat();
      bool vconcat();
      bool hrepeat();
      bool vrepeat();
      bool repeat();
      bool print();

    private:
      tree_t matrixTree_;
      std::istream& in_;
      std::ostream& out_;
      void fillRandom(Matrix< int >&);
      void fillNumber(Matrix< int >&, const int&);
      Matrix< int > dohconcat(const Matrix< int >&, const Matrix< int >&);
      Matrix< int > dovconcat(const Matrix< int >&, const Matrix< int >&);
      void insert(const std::string&, const Matrix< int >&);
  };
}

#endif
