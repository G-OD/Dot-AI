#ifndef POPULATION_H
#define POPULATION_H

#include <vector>

#include "Dot.hpp"

class Population
{
	public:
		int size = 0;
		std::vector<Dot> dots;
		double fitnessSum = 0;
		int bestDot = 0;
		int maxStep = 400;
		int generation = 0;

		Population(){}
		Population(int size);

		// Is everyone dead?
		bool isDead();

		// Set best dot index
		void setBestDot();

		// Calculate fitness sum
		void calculateFitnessSum(Vector2 goal);

		// Select dot randomly based on fitness
		Dot naturalSelection();

		// Select for the next generation
		void nextGeneration();

		// Mutate population
		void mutate();
};

#endif