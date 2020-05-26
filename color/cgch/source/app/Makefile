INC=../include
LIB=../lib
BIN=../bin
FLAGS= -O2 -Wall

cgch:
	cd ../; make; cd app
	gcc $(FLAGS) $@_extraction.c -o $(BIN)/$@_extraction -I$(INC) -L$(LIB) -ldescriptors -lm
	gcc $(FLAGS) $@_distance.c -o $(BIN)/$@_distance -I$(INC) -L$(LIB) -ldescriptors -lm
	gcc $@_plugin.c -fpic -shared -o $(BIN)/$@.so -I$(INC) -L$(LIB) -ldescriptors

clean:
	cd ../; make clean; cd app

