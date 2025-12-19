CXX = g++
EXECS = signal-echo
OBJS = se.o
CODES = se.cpp

all: $(EXECS)
	
copy: cpy.o
	$(CXX) se.o -o signal-echo
	
cpy.o:
	$(CXX) -c se.cpp -o se.o

clean:
	rm -f $(EXECS) $(OBJS)
