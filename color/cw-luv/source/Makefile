FLAGS= -O2 -Wall

cw-luv:
	gcc $(FLAGS) $@_extraction.c -o $@_extraction -lm
	gcc $(FLAGS) $@_distance.c -o $@_distance -lm
	gcc $@_plugin.c -fpic -shared -o $@.so

clean:
	rm *.so cw-luv_extraction cw-luv_distance

