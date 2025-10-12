CXX = g++
EXECS = complex
OBJS = main.o cn.o srt.o

all: $(EXECS)

complex: $(OBJS)
	$(CXX) $(OBJS) -o complex

cn.o: cn.cpp cn.h
	$(CXX) -c cn.cpp -o cn.o

srt.o: sorts.cpp sorts.h
	$(CXX) -c sorts.cpp -o srt.o
	
main.o: main.cpp cn.h sorts.h
	$(CXX) -c main.cpp -o main.o

clean:
	rm -f $(EXECS) $(OBJS)
