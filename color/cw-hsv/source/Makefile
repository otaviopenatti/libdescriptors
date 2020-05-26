FLAGS= -O2 -Wall

cw-hsv:
	gcc $(FLAGS) $@_extraction.c -o $@_extraction -lm
	gcc $(FLAGS) $@_distance.c -o $@_distance -lm
	gcc $@_plugin.c -fpic -shared -o $@.so

clean:
	rm *.so cw-hsv_extraction cw-hsv_distance

