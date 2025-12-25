CXX = g++
EXECS = chat-client chat-server
OBJS = cli.o srv.o
CODES = cli.cpp srv.cpp

all: $(EXECS)

chat-client: cli.o
	$(CXX) cli.o -o chat-client
	
chat-server: srv.o
	$(CXX) srv.o -o chat-server
	
cli.o: cli.cpp
	$(CXX) -c cli.cpp -o cli.o
	
srv.o: srv.cpp
	$(CXX) -c srv.cpp -o srv.o

clean:
	rm -f $(EXECS) $(OBJS)

