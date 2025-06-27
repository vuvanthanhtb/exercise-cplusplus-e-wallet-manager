CXX = g++
CXXFLAGS = -std=c++11 -Iinclude

SRC = main.cpp src/*.cpp src/utils/*.cpp
OUT = main

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

run: all
	./$(OUT)

clean:
	rm -f $(OUT)
