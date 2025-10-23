CXX = g++
EXECS = manual-append
OBJS = ma.o

all: $(EXECS)

manual-append: ma.o
	$(CXX) ma.o -o manual-append
	
ma.o:
	$(CXX) -c ma.cpp -o ma.o

clean:
	rm -f $(EXECS) $(OBJS)
