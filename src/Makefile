CXX      = g++
RM       = rm
CPPFLAGS = -Wall -Wextra -Werror -g -O2 -Wall -pedantic -std=c++11 -pthread
SRCS     = $(wildcard *.cpp) 
INC      = -I../src -I/home/michael/CS/git/modular-boost
LINK     = -L/home/michael/CS/git/modular-boost/stage/lib

all: $(SRCS:.cpp=)

.cpp: 
	$(CXX) $(CPPFLAGS) $(INC) $< -o $@.out

clean:
	$(RM) *.out
