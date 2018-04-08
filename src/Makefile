
LIBS=-lopenal
HEADERS="-I/usr/include/AL"
LIB_DIR="-L/usr/lib/x86_64-linux-gnu"
COMPILER_FLAGS=-Wall -g
CC=g++


default: main

clean:
	rm -f *.o ../out/main

util.o: util.cc util.h
	$(CC) ${COMPILER_FLAGS} -c util.cc -o util.o ${COMPILER_FLAGS} ${HEADERS} ${LIB_DIR} ${LIBS}

wavfile.o: wavfile.cc wavfile.h
	$(CC) ${COMPILER_FLAGS} -c wavfile.cc -o wavfile.o ${COMPILER_FLAGS} ${HEADERS} ${LIB_DIR} ${LIBS}

config.o: config.cc config.h
	$(CC) ${COMPILER_FLAGS} -c config.cc -o config.o ${COMPILER_FLAGS} ${HEADERS} ${LIB_DIR} ${LIBS}

main: main.cc wavfile.o config.o util.o
	g++  ${COMPILER_FLAGS}  main.cc util.o config.o wavfile.o -o ../out/main ${HEADERS} ${LIB_DIR} ${LIBS}

