CXX := g++
CXX_FLAGS := -Wall -static-libgcc -static-libstdc++ -Wextra -std=c++17 -ggdb

BIN 		:= 
SRC 		:= src
INCLUDE 	:= INCLUDE
LIB 		:= lib
LIBRARIES 	:=
EXECUTABLE 	:= validate

all: $(EXECUTABLE)
run: clear all
	clear
	@echo "Executing..."
	$(EXECUTABLE)


$(EXECUTABLE): $(SRC)/*.cpp
	@echo "Building..."
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES)
