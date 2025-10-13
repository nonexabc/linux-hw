CXX = g++
EXECS = read-file simple-copy hacker-rm redirect-cin manual-append
OBJS = read-file.o scp.o hr.o rdc.o ma.o
CODES = read-file.c scp.c hr.c rdc.cpp ma.cpp

all: $(EXECS)

read-file: read-file.o
	$(CXX) read-file.o -o read-file

read-file.o: read-file.c
	$(CXX) -c read-file.c -o read-file.o
	
simple-copy: scp.o
	$(CXX) scp.o -o simple-copy
	
scp.o:
	$(CXX) -c scp.c -o scp.o
	
hacker-rm: hr.o
	$(CXX) hr.o -o hacker-rm
	
hr.o:
	$(CXX) -c hr.c -o hr.o

redirect-cin: rdc.o
	$(CXX) rdc.o -o redirect-cin
	
rdc.o:
	$(CXX) -c rdc.cpp -o rdc.o

manual-append: ma.o
	$(CXX) ma.o -o manual-append
	
ma.o:
	$(CXX) -c ma.cpp -o ma.o

clean:
	rm -f $(EXECS) $(OBJS)
