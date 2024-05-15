all: main

main: main.cpp src/*
	g++ -lncurses -o main.cpp 