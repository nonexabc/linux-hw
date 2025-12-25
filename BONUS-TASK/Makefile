FNAME = cli
EXECNAME = http-cli

CXX = g++
EXECS = ${EXECNAME}
OBJS = ${FNAME}.o
CODES = ${FNAME}.cpp

all: $(EXECS)
	
${EXECNAME}: ${FNAME}.o
	$(CXX) ${FNAME}.o -o ${EXECNAME}
	
${FNAME}.o:
	$(CXX) -c ${FNAME}.cpp -o ${FNAME}.o

clean:
	rm -f $(EXECS) $(OBJS)
