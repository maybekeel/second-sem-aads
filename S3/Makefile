hidecmd := @
RM = rm -rf

CXX = g++
CXXFLAGS += -g
CPPFLAGS += -Wall -Wextra -Werror -Wno-missing-field-initializers -Wold-style-cast
CPPFLAGS += -std=gnu++14

SOURCES := $(wildcard *.cpp) $(wildcard ../common/*.cpp)
OBJECTS := $(patsubst %.cpp,%.o,$(SOURCES))
DEPENDS := $(patsubst %.cpp,%.d,$(SOURCES))

include := ../common

EXECUTABLE = out

.PHONY: all clean

all: out

clean:
		@echo "[CLEAN UP]"
		$(hidecmd)$(RM) $(OBJECTS) $(DEPENDS) out

out: $(OBJECTS)
		@echo "[LINK] $(OBJECTS)"
		$(hidecmd)$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

-include $(DEPENDS)

%.o: %.cpp Makefile
		@echo "[C++ ] $@"
		$(hidecmd)$(CXX) $(CPPFLAGS) $(addprefix -I, $(include)) $(CXXFLAGS) -MMD -MP -c $< -o $@

