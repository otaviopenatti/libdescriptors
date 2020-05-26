LIB=./lib
INCLUDE=./include
SRC=./src
OBJ=./obj
BIN=./bin
APP=./app
FLAGS= -O2 -Wall -fPIC -g
CC=gcc

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
$(OBJ)/correlogram.o \
$(OBJ)/correlogram_descriptors.o

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
$(OBJ)/correlogram.o \
$(OBJ)/correlogram_descriptors.o


$(OBJ)/common.o: $(SRC)/common.c
	$(CC)  $(FLAGS) -c $(SRC)/common.c -I$(INCLUDE) \
	-o $(OBJ)/common.o 

$(OBJ)/comptime.o: $(SRC)/comptime.c
	$(CC)  $(FLAGS) -c $(SRC)/comptime.c -I$(INCLUDE) \
	-o $(OBJ)/comptime.o 

$(OBJ)/heap.o: $(SRC)/heap.c
	$(CC)  $(FLAGS) -c $(SRC)/heap.c -I$(INCLUDE) \
	-o $(OBJ)/heap.o 

$(OBJ)/queue.o: $(SRC)/queue.c
	$(CC)  $(FLAGS) -c $(SRC)/queue.c -I$(INCLUDE) \
	-o $(OBJ)/queue.o 

$(OBJ)/sort.o: $(SRC)/sort.c
	$(CC)  $(FLAGS) -c $(SRC)/sort.c -I$(INCLUDE) \
	-o $(OBJ)/sort.o 

$(OBJ)/stack.o: $(SRC)/stack.c
	$(CC)  $(FLAGS) -c $(SRC)/stack.c -I$(INCLUDE) \
	-o $(OBJ)/stack.o 

$(OBJ)/set.o: $(SRC)/set.c
	$(CC)  $(FLAGS) -c $(SRC)/set.c -I$(INCLUDE) \
	-o $(OBJ)/set.o 

$(OBJ)/image.o: $(SRC)/image.c
	$(CC)  $(FLAGS) -c $(SRC)/image.c -I$(INCLUDE) \
	-o $(OBJ)/image.o 

$(OBJ)/cimage.o: $(SRC)/cimage.c
	$(CC)  $(FLAGS) -c $(SRC)/cimage.c -I$(INCLUDE) \
	-o $(OBJ)/cimage.o 

$(OBJ)/adjacency.o: $(SRC)/adjacency.c
	$(CC)  $(FLAGS) -c $(SRC)/adjacency.c -I$(INCLUDE) \
	-o $(OBJ)/adjacency.o 

$(OBJ)/mathematics.o: $(SRC)/mathematics.c
	$(CC)  $(FLAGS) -c $(SRC)/mathematics.c -I$(INCLUDE) \
	-o $(OBJ)/mathematics.o 

$(OBJ)/color.o: $(SRC)/color.c
	$(CC)  $(FLAGS) -c $(SRC)/color.c -I$(INCLUDE) \
	-o $(OBJ)/color.o

$(OBJ)/featurevector.o: $(SRC)/featurevector.c
	$(CC)  $(FLAGS) -c $(SRC)/featurevector.c -I$(INCLUDE) \
	-o $(OBJ)/featurevector.o

$(OBJ)/metrics.o: $(SRC)/metrics.c
	$(CC)  $(FLAGS) -c $(SRC)/metrics.c -I$(INCLUDE) \
	-o $(OBJ)/metrics.o

$(OBJ)/histogram.o: $(SRC)/histogram.c
	$(CC)  $(FLAGS) -c $(SRC)/histogram.c -I$(INCLUDE) \
	-o $(OBJ)/histogram.o

$(OBJ)/correlogram.o: $(SRC)/correlogram.c
	$(CC)  $(FLAGS) -c $(SRC)/correlogram.c -I$(INCLUDE) \
	-o $(OBJ)/correlogram.o

$(OBJ)/correlogram_descriptors.o: $(SRC)/correlogram_descriptors.c
	$(CC)  $(FLAGS) -c $(SRC)/correlogram_descriptors.c -I$(INCLUDE) \
	-o $(OBJ)/correlogram_descriptors.o

clean: 
	rm $(LIB)/lib*.a; rm $(OBJ)/*.o; rm $(BIN)/*

