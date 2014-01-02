CXX      = g++
RM       = rm
CPPFLAGS = -Wall -Wextra -Werror -g -O2 -Wall -pedantic -std=c++11 -pthread
SRCS     = $(wildcard *.cpp) 
INC      = -I../src

all: $(SRCS:.cpp=)

.cpp: 
	$(CXX) $(CPPFLAGS) $(INC) $(OBJS) $< -o $@.out

clean:
	$(RM) *.out
