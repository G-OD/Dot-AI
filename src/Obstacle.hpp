#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "Vector2.hpp"

class Obstacle
{
	public:
		Vector2 pos;
		float width;
		float height;

		Obstacle(float x, float y, float width, float height);
};

#endif