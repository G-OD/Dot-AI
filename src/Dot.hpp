#ifndef DOT_H
#define DOT_H

#include <cmath>

#include "Vector2.hpp"
#include "Brain.hpp"

class Dot
{
	public:
		Vector2 pos;
		Vector2 vel;
		Brain brain = Brain(400);
		double fitness = 0;
		bool dead = false;
		bool successful = false;

		Dot(int brainSize);

		// Calculate fitness
		void calculateFitness(Vector2 goal);

		// Clone dot
		Dot clone(int brainSize);
};

#endif