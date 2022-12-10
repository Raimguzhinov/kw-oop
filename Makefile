bin/sfml : src/main.cpp
	clang++ -w -std=c++11 src/main.cpp -I include -o bin/sfml -L lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

	
run :
	./bin/sfml

