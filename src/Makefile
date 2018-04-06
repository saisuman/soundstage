
LIBS=-lopenal
HEADERS="-I/usr/include/AL"
LIB_DIR="-L/usr/lib/x86_64-linux-gnu"
COMPILER_FLAGS=-Wall -g
CC=g++


default: main


wavfile.o: wavfile.cc wavfile.h
	$(CC) -c wavfile.cc -o wavfile.o ${COMPILER_FLAGS}

main: main.cc wavfile.o
	$(CC)  ${COMPILER_FLAGS}  main.cc  wavfile.o -o ../out/main $(HEADERS) $(LIB_DIR) $(LIBS)

	