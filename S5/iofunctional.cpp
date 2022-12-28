#include "iofunctional.h"
#include <string>

void ivlicheva::Print::operator()(const std::pair< long long, std::string >& data)
{
  if (!tree_.empty())
  {
    tree_ += ' ';
  }
  tree_ += data.second;
}

const std::string& ivlicheva::Print::getTree() const
{
  return tree_;
}
