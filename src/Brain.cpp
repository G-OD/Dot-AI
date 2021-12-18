#include "Brain.hpp"

Brain::Brain(int size)
{
	this->size = size;
}

void Brain::randomize()
{
	float tau = 2 * M_PI;
	for (int i = 0; i < size; ++i)
	{
		float randomAngle = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / tau));
		directions.push_back(Vector2( cos(randomAngle), sin(randomAngle) ));
	}
}

void Brain::mutate()
{
	float tau = 2 * M_PI;
	int mutateChance = 1; // 1 Percent

	for (auto it = directions.begin(); it != directions.end(); ++it)
	{
		int random = rand() % 100; // 0-99

		if (random <= mutateChance)
		{
			float randomAngle = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / tau));
			directions[it - directions.begin()] = Vector2( cos(randomAngle), sin(randomAngle) );
		}
	}
}