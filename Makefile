CXX=g++

CXXFLAGS=-Wall -Iinclude

LIBS=-lncurses -lpthread

SRC=$(wildcard src/**/*.cpp) $(wildcard src/*.cpp)

OUT=kirby

all:
	$(CXX) $(SRC) -o $(OUT) $(CXXFLAGS) $(LIBS)

run:
	./$(OUT)

clean:
	rm -f $(OUT)
