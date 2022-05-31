test: compile 
	./MT 1001 MT_divideby3.txt
	./MT 01100 MT_ZEROFIRST.txt
	./MT 1101 MT_addOne.txt
	./MT 00000001101011 MT_TO_BINARY.txt
	./MT 10000001101011 MT_TO_BINARY.txt
compile: MT.c
	gcc -o MT MT.c -Wall

