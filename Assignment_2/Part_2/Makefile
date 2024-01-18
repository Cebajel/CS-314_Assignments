#Author: Cebajel Tanan (210010055)
#Title: CS 314 Assignment 2 - Part 2
#Date: January 17th, 2024

compile: half twice square

half: half.c
	clang $^ -o $@

twice: twice.c
	clang $^ -o $@

square: square.c
	clang $^ -o $@

clear:
	rm -f half twice square

.PHONY:clear compile