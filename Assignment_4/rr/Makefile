BIN=./bin
OBJ=./obj
SRC=./src
INC=./include
CC=g++
INPUT=./test/process1.txt

compile: $(OBJ)/rr

$(BIN)/scheduler: $(SRC)/scheduler.cpp $(INC)/scheduler.h $(INC)/process.h $(INC)/cpu.h $(INC)/clock.h $(INC)/io.h
	$(CC) -c -o $@ $< -I$(INC)

$(BIN)/process: $(SRC)/process.cpp $(INC)/process.h $(INC)/clock.h
	$(CC) -c -o $@ $< -I$(INC)

$(BIN)/cpu: $(SRC)/cpu.cpp $(INC)/cpu.h $(INC)/process.h $(INC)/clock.h
	$(CC) -c -o $@ $< -I$(INC)

$(BIN)/io: $(SRC)/io.cpp $(INC)/io.h $(INC)/process.h $(INC)/clock.h
	$(CC) -c -o $@ $< -I$(INC)

$(BIN)/clock: $(SRC)/clock.cpp $(INC)/clock.h
	$(CC) -c -o $@ $< -I$(INC)


$(OBJ)/rr: $(BIN)/scheduler $(BIN)/process $(BIN)/cpu $(BIN)/io $(BIN)/clock $(SRC)/main.cpp
	$(CC) -o $@ $^ -I$(INC)
	$@ $(INPUT)

clean:
	rm -f ./obj/* ./bin/*

.PHONY: compile clean