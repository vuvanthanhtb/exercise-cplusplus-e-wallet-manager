CXX = g++
CXXFLAGS = -std=c++11 -Iinclude

# Phát hiện nền tảng
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
    OS = LINUX
    LDFLAGS = -lcurl
endif

ifeq ($(UNAME_S),Darwin)
    OS = MACOS
    CURL_PATH := $(shell brew --prefix curl)
    CXXFLAGS += -I$(CURL_PATH)/include
    LDFLAGS = -L$(CURL_PATH)/lib -lcurl
endif

ifeq ($(OS),)
    ifeq ($(OS),Windows_NT)
        OS = WINDOWS
        CXX = g++  # Hoặc x86_64-w64-mingw32-g++
        CXXFLAGS += -I"C:/curl/include"
        LDFLAGS = -L"C:/curl/lib" -lcurl
    endif
endif

SRC = main.cpp $(wildcard src/*.cpp src/utils/*.cpp)
OUT = main

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)

run: all
ifeq ($(OS),WINDOWS)
	$(OUT).exe
else
	./$(OUT)
endif

clean:
	rm -f $(OUT) $(OUT).exe
