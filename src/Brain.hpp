#ifndef BRAIN_H
#define BRAIN_H

#include <vector>
#include <random>
#include <cmath>

#include "Vector2.hpp"

class Brain
{
	public:
		int size;
		std::vector<Vector2> directions;
		int step = 0;

		Brain(int size);

		// Randomize thought process
		void randomize();

		// Mutate existing thought process
		void mutate();
};

#endif