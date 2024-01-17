#Author: Cebajel Tanan (210010055)
#Title: CS 314 Assignment 2 - Part 2
#Date: January 17th, 2024

lab2_part2: lab2_part2.c half twice square
	clang $< -o $@

half: half.c
	clang $^ -o $@

twice: twice.c
	clang $^ -o $@

square: square.c
	clang $^ -o $@

clear:
	rm -f lab2_part2 half twice square

.phony:clear