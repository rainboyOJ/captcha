test: test.cpp njnaruto_bmf.hpp lodepng.cpp
	g++ -g -o test.out test.cpp lodepng.cpp -I../
	./test.out
decode:
	g++ -o decode decode.cpp lodepng.cpp

