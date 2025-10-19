CXX = g++
CFLAGS = -std=c++20
EXECS = interactive-shell
OBJS = is.o
CODES = is.cpp

all: $(EXECS)

interactive-shell: is.o
	$(CXX) $(CFLAGS) is.o -o interactive-shell

is.o: is.cpp
	$(CXX) $(CFLAGS) -c is.cpp -o is.o

clean:
	rm -f $(EXECS) $(OBJS)
