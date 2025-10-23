CXX = g++
EXECS = simple-copy
OBJS = scp.o

all: $(EXECS)
	
simple-copy: scp.o
	$(CXX) scp.o -o simple-copy
	
scp.o:
	$(CXX) -c scp.c -o scp.o

clean:
	rm -f $(EXECS) $(OBJS)
