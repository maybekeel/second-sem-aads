#ifndef IOMESSAGES_H
#define IOMESSAGES_H

#include <iosfwd>

namespace ivlicheva
{
  void outputMessageInvalidCommand(std::ostream&);
  void outputMessageEmpty(std::ostream&);
  void outputMessageTrue(std::ostream&);
  void outputMessageFalse(std::ostream&);
}

#endif
