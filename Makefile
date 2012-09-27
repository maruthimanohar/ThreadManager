CC = g++
LIBS = -lpthread

all: test1 test2

test1:
	${CC} -o test1 test1.cc -std=c++0x ${LIBS}

test2:
	${CC} -o test2 test2.cc -std=c++0x ${LIBS}

clean:
	rm -rf test1 test2
