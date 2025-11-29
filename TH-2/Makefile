CXX = g++
EXECS = demo-application
OBJS = da.o tpool.o libtpool.so
CODES = da.cpp tpool.cpp

all: $(EXECS)

demo-application: da.o libtpool.so
	$(CXX) -o demo-application da.o -L. -ltpool -Wl,-rpath=.

libtpool.so: tpool.o
	$(CXX) -shared tpool.o -o libtpool.so

tpool.o: tpool.cpp
	$(CXX) -fPIC -c tpool.cpp -o tpool.o

da.o: da.cpp
	$(CXX) -fPIC -c da.cpp -o da.o

clean:
	rm -f $(EXECS) $(OBJS)
