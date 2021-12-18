#include "Dot.hpp"

Dot::Dot(int brainSize)
{
	pos = Vector2(0.0f, -450.0f);
	vel = Vector2();
	
	brain = Brain(brainSize);
}

void Dot::calculateFitness(Vector2 goal)
{
	if (successful)
	{
		fitness = 10000.0f / (double)(brain.step * brain.step);
	}
	else
	{
		double dist = sqrt( pow(goal.x - pos.x, 2) + pow(goal.y - pos.y, 2) );
		fitness = 1.0f / (dist * dist);
	}	
}

Dot Dot::clone(int brainSize)
{
	Dot cloneDot = Dot(brainSize);

	for (int i = 0; i < brainSize; ++i)
		cloneDot.brain.directions.push_back(brain.directions[i]);

	return cloneDot;
}