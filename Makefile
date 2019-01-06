CC=g++
LINK_TARGET=main.exe
OBJS=main.o

test: test.cpp Pool.h
	$(CC) -O3 -std=c++17 -o test test.cpp

clean:
	rm main.o main.exe
