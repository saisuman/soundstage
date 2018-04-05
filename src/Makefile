
LIBS=-lopenal
HEADERS="-I/usr/include/AL"
LIB_DIR="-L/usr/lib/x86_64-linux-gnu"
COMPILER_FLAGS=-Wall

default: main


wavfile.o: wavfile.cc wavfile.h
	g++ -c wavfile.cc -o wavfile.o ${COMPILER_FLAGS}

main: main.cc wavfile.o
	g++  ${COMPILER_FLAGS}  main.cc  wavfile.o -o main ${HEADERS} ${LIB_DIR} ${LIBS}
	