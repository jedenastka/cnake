all:
	g++ -lncurses cnake.cpp -o cnake -static-libstdc++ -static-libgcc -pthread
