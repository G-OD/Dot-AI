#include "Population.hpp"

Population::Population(int size)
{
	this->size = size;

	for (int i = 0; i < size; ++i)
	{
		dots.push_back(Dot(maxStep));
		dots[i].brain.randomize();
	}
}

bool Population::isDead()
{
	for (auto it = dots.begin(); it != dots.end(); ++it)
	{
		if (it->dead == false && it->successful == false)
		{
			return false;
		}
	}

	return true;
}

void Population::setBestDot()
{
	for (auto it = dots.begin(); it != dots.end(); ++it)
	{
		if (it->fitness > dots[bestDot].fitness)
		{
			bestDot = it - dots.begin();
		}
	}

	if (dots[bestDot].successful && dots[bestDot].brain.step < maxStep)
	{
		maxStep = dots[bestDot].brain.step;
	}
}

void Population::calculateFitnessSum(Vector2 goal)
{
	fitnessSum = 0;
	for (auto it = dots.begin(); it != dots.end(); ++it)
	{
		it->calculateFitness(goal);
		fitnessSum += it->fitness;
	}
}

Dot Population::naturalSelection()
{
	// Generate number between 0 and fitness sum
	double random = static_cast<double>(rand()) / (static_cast<double>(RAND_MAX / fitnessSum));
	double runningSum = 0;

	for (auto it = dots.begin(); it != dots.end(); ++it)
	{
		runningSum += it->fitness;
		if (runningSum > random)
			return *it;
	}

	// Shouldn't reach here
	Dot randomDot = Dot(maxStep);
	return randomDot;
}

void Population::nextGeneration()
{
	std::vector<Dot> newDots;

	// Add best dot directly into the next generation
	setBestDot();
	Dot bestDotEver = dots[bestDot].clone(maxStep);
	newDots.push_back(bestDotEver);

	for (int i = 1; i < size; ++i)
	{
		// Select dot randomly based on fitness
		Dot dot = naturalSelection();
		Dot clone = dot.clone(maxStep);
		newDots.push_back(clone);
	}

	dots.clear();
	dots = newDots;
	++generation;
}

void Population::mutate()
{
	// Mutate all but first (best dot)
	for (auto it = dots.begin() + 1; it != dots.end(); ++it)
		it->brain.mutate();
}