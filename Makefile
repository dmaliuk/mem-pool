CC=g++
SOURCES=test.cpp Task.cpp
HEADERS=Pool.h Task.h


test: $(HEADERS) $(SOURCES)
	$(CC) -O3 -std=c++17 -o test $(SOURCES)
