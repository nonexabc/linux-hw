CXX = g++
EXECS = read-file simple-copy hacker-rm
OBJS = read-file.o scp.o hr.o
CODES = read-file.c scp.c hr.c

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

clean:
	rm -f $(EXECS) $(OBJS)
