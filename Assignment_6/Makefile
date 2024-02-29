SRC=./src
OUT=./output
BIN=./bin
IN=./input
sample=sample_pp3_5mb.ppm
CFLAGS=-std=c++11 -Wall
part1: $(SRC)/part1.cpp
	g++ $(CFLAGS) $< -o $(BIN)/$@
	time $(BIN)/part1 $(IN)/$(sample) $(OUT)/output_part1.ppm

part2_1a: $(SRC)/part2_1a.cpp
	g++ $(CFLAGS) $< -o $(BIN)/$@
	time $(BIN)/$@ $(IN)/$(sample) $(OUT)/output_part2_1a.ppm

part2_1b: $(SRC)/part2_1b.cpp
	g++ $(CFLAGS) $< -o $(BIN)/$@
	time $(BIN)/$@ $(IN)/$(sample) $(OUT)/output_part2_1b.ppm

part2_2: $(SRC)/part2_2.cpp
	g++ $(CFLAGS) $< -o $(BIN)/$@
	time $(BIN)/$@ $(IN)/$(sample) $(OUT)/output_part2_2.ppm

part2_3: $(SRC)/part2_3.cpp
	g++ $(CFLAGS) $< -o $(BIN)/$@
	time $(BIN)/$@ $(IN)/$(sample) $(OUT)/output_part2_3.ppm

clear:
	rm -f ./output/*
	rm -f ./bin/*

.PHONY: part1 part2_1a part2_1b part2_2 part2_3 clear