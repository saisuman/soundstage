
LIBS="-lpulse-simple -lpulse"
HEADERS="-I/usr/include/pulse"

default: main


wavfile.o: wavfile.cc wavfile.h
	g++ -c wavfile.cc -o wavfile.o ${COMPILER_FLAGS}

main: main.cc wavfile.o
	g++  ${COMPILER_FLAGS}  main.cc  wavfile.o -o main ${HEADERS} -lpulse -lpulse-simple
	