LIB=./lib
INCLUDE=./include
SRC=./src
OBJ=./obj
BIN=./bin
APP=./app
FLAGS= -O2 -Wall -fPIC

libdescriptors: $(LIB)/libdescriptors.a;
	@echo "Descriptors library built!"

$(LIB)/libdescriptors.a: \
$(OBJ)/common.o \
$(OBJ)/comptime.o \
$(OBJ)/heap.o \
$(OBJ)/queue.o \
$(OBJ)/sort.o \
$(OBJ)/set.o \
$(OBJ)/stack.o \
$(OBJ)/image.o \
$(OBJ)/cimage.o \
$(OBJ)/adjacency.o \
$(OBJ)/mathematics.o \
$(OBJ)/color.o \
$(OBJ)/featurevector.o \
$(OBJ)/metrics.o \
$(OBJ)/histogram.o \
$(OBJ)/irm.o \
$(OBJ)/cbc.o

	ar csr $(LIB)/libdescriptors.a \
$(OBJ)/common.o \
$(OBJ)/comptime.o \
$(OBJ)/heap.o \
$(OBJ)/queue.o \
$(OBJ)/sort.o \
$(OBJ)/set.o \
$(OBJ)/stack.o \
$(OBJ)/image.o \
$(OBJ)/cimage.o \
$(OBJ)/adjacency.o \
$(OBJ)/mathematics.o \
$(OBJ)/color.o \
$(OBJ)/featurevector.o \
$(OBJ)/metrics.o \
$(OBJ)/histogram.o \
$(OBJ)/irm.o \
$(OBJ)/cbc.o

$(OBJ)/common.o: $(SRC)/common.c
	gcc $(FLAGS) -c $(SRC)/common.c -I$(INCLUDE) \
	-o $(OBJ)/common.o 

$(OBJ)/comptime.o: $(SRC)/comptime.c
	gcc $(FLAGS) -c $(SRC)/comptime.c -I$(INCLUDE) \
	-o $(OBJ)/comptime.o 

$(OBJ)/heap.o: $(SRC)/heap.c
	gcc $(FLAGS) -c $(SRC)/heap.c -I$(INCLUDE) \
	-o $(OBJ)/heap.o 

$(OBJ)/queue.o: $(SRC)/queue.c
	gcc $(FLAGS) -c $(SRC)/queue.c -I$(INCLUDE) \
	-o $(OBJ)/queue.o 

$(OBJ)/sort.o: $(SRC)/sort.c
	gcc $(FLAGS) -c $(SRC)/sort.c -I$(INCLUDE) \
	-o $(OBJ)/sort.o 

$(OBJ)/stack.o: $(SRC)/stack.c
	gcc $(FLAGS) -c $(SRC)/stack.c -I$(INCLUDE) \
	-o $(OBJ)/stack.o 

$(OBJ)/set.o: $(SRC)/set.c
	gcc $(FLAGS) -c $(SRC)/set.c -I$(INCLUDE) \
	-o $(OBJ)/set.o 

$(OBJ)/image.o: $(SRC)/image.c
	gcc $(FLAGS) -c $(SRC)/image.c -I$(INCLUDE) \
	-o $(OBJ)/image.o 

$(OBJ)/cimage.o: $(SRC)/cimage.c
	gcc $(FLAGS) -c $(SRC)/cimage.c -I$(INCLUDE) \
	-o $(OBJ)/cimage.o 

$(OBJ)/adjacency.o: $(SRC)/adjacency.c
	gcc $(FLAGS) -c $(SRC)/adjacency.c -I$(INCLUDE) \
	-o $(OBJ)/adjacency.o 

$(OBJ)/mathematics.o: $(SRC)/mathematics.c
	gcc $(FLAGS) -c $(SRC)/mathematics.c -I$(INCLUDE) \
	-o $(OBJ)/mathematics.o 

$(OBJ)/color.o: $(SRC)/color.c
	gcc $(FLAGS) -c $(SRC)/color.c -I$(INCLUDE) \
	-o $(OBJ)/color.o

$(OBJ)/featurevector.o: $(SRC)/featurevector.c
	gcc $(FLAGS) -c $(SRC)/featurevector.c -I$(INCLUDE) \
	-o $(OBJ)/featurevector.o

$(OBJ)/metrics.o: $(SRC)/metrics.c
	gcc $(FLAGS) -c $(SRC)/metrics.c -I$(INCLUDE) \
	-o $(OBJ)/metrics.o

$(OBJ)/histogram.o: $(SRC)/histogram.c
	gcc $(FLAGS) -c $(SRC)/histogram.c -I$(INCLUDE) \
	-o $(OBJ)/histogram.o

$(OBJ)/irm.o: $(SRC)/irm.c
	gcc $(FLAGS) -c $(SRC)/irm.c -I$(INCLUDE) \
	-o $(OBJ)/irm.o

$(OBJ)/cbc.o: $(SRC)/cbc.c
	gcc $(FLAGS) -c $(SRC)/cbc.c -I$(INCLUDE) \
	-o $(OBJ)/cbc.o

clean: 
	rm $(LIB)/lib*.a; rm $(OBJ)/*.o; rm $(BIN)/*

