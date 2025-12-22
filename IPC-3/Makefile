CXX = g++
EXECS = first second
OBJS = f.o s.o sharr.o
CODES = f.cpp s.cpp sharr.cpp

all: $(EXECS)

first: f.o sharr.o
	$(CXX) -o first f.o sharr.o

second: s.o sharr.o
	$(CXX) -o second s.o sharr.o

s.o: s.cpp sharr.cpp
	$(CXX) -c s.cpp -o s.o

f.o: f.cpp sharr.cpp
	$(CXX) -c f.cpp -o f.o

sharr.o: sharr.cpp
	$(CXX) -c sharr.cpp -o sharr.o

clean:
	rm -f $(EXECS) $(OBJS)
