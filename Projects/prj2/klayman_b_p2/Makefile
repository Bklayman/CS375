CXX = g++
CXXFLAGS = -g -Wall -std=c++17

CXX_FILES = $(wildcard *.cc)

OFILES = $(CXX_FILES:cc=o)

all:		clean submission

submission:	$(OFILES)
	$(CXX) $(CXXFLAGS) submission.o -o $@

clean:
	rm -f *~ *.o submission
