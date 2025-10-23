CXX = g++
EXECS = redirect-cin
OBJS = rdc.o

all: $(EXECS)

redirect-cin: rdc.o
	$(CXX) rdc.o -o redirect-cin
	
rdc.o:
	$(CXX) -c rdc.cpp -o rdc.o

clean:
	rm -f $(EXECS) $(OBJS)
