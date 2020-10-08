CXX = g++
CXXFLAGS = -g -Wall -std=c++17

CXX_FILES = $(wildcard *.cc)

OFILES = $(CXX_FILES:cc=o)

all:		clean program1

program1:	$(OFILES)
	$(CXX) $(CXXFLAGS) program1.o -o $@

clean:
	rm -f *~ *.o program1
