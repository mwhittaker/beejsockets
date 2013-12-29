CXX      = g++
RM       = rm
CPPFLAGS = -Wall -Wextra -Werror -g -O2 -Wall -pedantic -std=c++11 -pthread
SRCS     = $(wildcard *.cpp) 
HEADERS  = $(wildcard *.h) 

all: $(SRCS:.cpp=)

.cpp:
	$(CXX) -c $(CPPFLAGS) $< 

.o:
	@#

clean:
	-$(RM) *.o
