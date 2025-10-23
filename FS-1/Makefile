CXX = g++
EXECS = read-file
OBJS = read-file.o

all: $(EXECS)

read-file: read-file.o
	$(CXX) read-file.o -o read-file

read-file.o: read-file.c
	$(CXX) -c read-file.c -o read-file.o
	
clean:
	rm -f $(EXECS) $(OBJS)
