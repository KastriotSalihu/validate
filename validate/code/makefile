CXX := g++
CXX_FLAGS := -Wall -Wextra -std=c++17 -ggdb

BIN 		:= ..
SRC 		:= src
INCLUDE 	:= INCLUDE
LIB 		:= lib
LIBRARIES 	:=
EXECUTABLE 	:= validate

all: $(BIN)/$(EXECUTABLE)
run: clear all
	clear
	@echo "Executing..."
	./$(BIN)/$(EXECUTABLE)


$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	@echo "Building..."
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES)

clean:
	@echo "Clearing..."
	-rm $(BIN)/*