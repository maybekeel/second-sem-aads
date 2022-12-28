# Version 2.1

hidecmd := @

CPPFLAGS += -Wall -Wextra -Werror -Wno-missing-field-initializers -Wold-style-cast
CXXFLAGS += -g

CPPFLAGS += -std=gnu++14

CC=g++
LDFLAGS=
SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=out

all: clean $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
		@echo "[LINK] $(OBJECTS)"
		$(hidecmd)$(CC) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
		@echo "[C++ ] $@"
		$(hidecmd)$(CC) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) -MMD -MP -c -o $@ $< 

clean:
		rm -rf *.o out *.d