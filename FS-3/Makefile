CXX = g++
EXECS = hacker-rm
OBJS = hr.o

all: $(EXECS)
	
hacker-rm: hr.o
	$(CXX) hr.o -o hacker-rm
	
hr.o:
	$(CXX) -c hr.c -o hr.o

clean:
	rm -f $(EXECS) $(OBJS)
