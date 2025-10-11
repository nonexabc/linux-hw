CXX = g++
EXECS = do-command
OBJS = do-command.o
CODES = do-command.c

all: $(EXECS)

do-command: do-command.o
	$(CXX) do-command.o -o do-command

do-command.o: do-command.c
	$(CXX) -c do-command.c -o do-command.o

clean:
	rm -f $(EXECS) $(OBJS)
