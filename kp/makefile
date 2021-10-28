all: p 

p: child main.cpp commands.hpp childc
	g++ -o p main.cpp -lzmq

child: B.cpp commands.hpp
	g++ -o child B.cpp -lzmq

childc: C.cpp commands.hpp
	g++ -o childc C.cpp -lzmq

clean:
	rm p child