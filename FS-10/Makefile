CXX = g++
EXECS = copy
OBJS = cpy.o
CODES = cpy.cpp

all: $(EXECS)
	
copy: cpy.o
	$(CXX) cpy.o -o copy
	
cpy.o:
	$(CXX) -c cpy.cpp -o cpy.o

clean:
	rm -f $(EXECS) $(OBJS)
