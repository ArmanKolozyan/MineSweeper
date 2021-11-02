all: main.o cell.o handleInput.o printing.o
	gcc main.o cell.o handleInput.o printing.o -o MineSweeper

main.o: MineSweeper.c cell.h datatypes.h handleInput.h macros.h printing.h
	gcc MineSweeper.c -c -o main.o

cell.o: cell.c cell.h macros.h
	gcc cell.c -c -o cell.o

handleInput.o: handleInput.c handleInput.h macros.h 
	gcc handleInput.c -c -o handleInput.o

printing.o: printing.c printing.h macros.h 
	gcc printing.c -c -o printing.o
