#include "Obstacle.hpp"

Obstacle::Obstacle(float x, float y, float width, float height)
{
	this->pos = Vector2(x, y);
	this->width = width;
	this->height = height;
}