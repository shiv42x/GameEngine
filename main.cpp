#include <SFML\Graphics.hpp>
#include <iostream>
#include "Game.h"

int main()
{
	Vec2 v1(1, 3);
	Vec2 v2(2, 3);

	v1 += v2;
	std::cout
		<< v1.x
		<< ' '
		<< v1.y
		<< '\n';

	Game g("config.txt");
	g.run();
}