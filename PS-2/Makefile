CXX = g++
CFLAGS = -std=c++20
EXECS = interactive-shell interactive-shell2
OBJS = is.o is2.o
CODES = is.cpp is2.cpp

all: $(EXECS)

interactive-shell: is.o
	$(CXX) $(CFLAGS) is.o -o interactive-shell

is.o: is.cpp
	$(CXX) $(CFLAGS) -c is.cpp -o is.o
	
interactive-shell2: is2.o
	$(CXX) $(CFLAGS) is2.o -o interactive-shell2

is2.o: is2.cpp
	$(CXX) $(CFLAGS) -c is2.cpp -o is2.o

clean:
	rm -f $(EXECS) $(OBJS)
