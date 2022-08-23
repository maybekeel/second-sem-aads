#ifndef COMMANDS_H
#define COMMANDS_H

#include <iosfwd>
#include <map>
#include "matrix.hpp"

namespace ivlicheva
{
  struct Commands
  {
    using map_t = std::map< std::string, Matrix< int > >;
    public:
      Commands(const map_t&, std::istream&, std::ostream&);
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
      map_t matrixMap_;
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
