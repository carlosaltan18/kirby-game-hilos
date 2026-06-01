CXX=g++

CXXFLAGS=-Wall -Iinclude

LIBS=-lncurses -lpthread

SRC=$(wildcard src/**/*.cpp) $(wildcard src/*.cpp)

OUT=kirby
WEB_SERVER_OUT=kirby_web_server
WEB_SERVER_SRC=tools/web_bridge/WebBridgeServer.cpp

all:
	$(CXX) $(SRC) -o $(OUT) $(CXXFLAGS) $(LIBS)

run:
	./$(OUT)

web-server:
	$(CXX) $(WEB_SERVER_SRC) -o $(WEB_SERVER_OUT) -Wall -lpthread

run-web:
	./$(WEB_SERVER_OUT)

clean:
	rm -f $(OUT) $(WEB_SERVER_OUT)
