CXX = g++
EXECS = array-sum
OBJS = as.o
CODES = as.cpp

all: $(EXECS)

array-sum: as.o
	$(CXX) as.o -o array-sum

as.o: as.cpp
	$(CXX) -c as.cpp -o as.o

clean:
	rm -f $(EXECS) $(OBJS)
