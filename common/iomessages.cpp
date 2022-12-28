#include "iomessages.h"
#include <iostream>

void ivlicheva::outputMessageInvalidCommand(std::ostream& stream)
{
  stream << "<INVALID COMMAND>\n";
}

void ivlicheva::outputMessageEmpty(std::ostream& stream)
{
  stream << "<EMPTY>\n";
}

void ivlicheva::outputMessageTrue(std::ostream& stream)
{
  stream << "<TRUE>\n";
}

void ivlicheva::outputMessageFalse(std::ostream& stream)
{
  stream << "<FALSE>\n";
}
