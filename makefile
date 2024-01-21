all:
	g++ main.cpp imgui/*.cpp -std=c++20 -o gol -lsfml-graphics -lsfml-window -lsfml-window -lsfml-system -lsfml-audio -lGL