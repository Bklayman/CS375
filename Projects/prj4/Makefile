CXX = g++
CXXFLAGS = -g -Wall -std=c++17

CXX_FILES = $(wildcard *.cc)

OFILES = $(CXX_FILES:cc=o)

all:		clean program4

program4:	$(OFILES)
	$(CXX) $(CXXFLAGS) program4.o -o $@

clean:
	rm -f *~ *.o program4
