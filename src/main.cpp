// C++ Standard libraries
#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <ctime>
//

// FreeGLUT
#include <GL/freeglut.h>
#include <GL/gl.h>
//

// Made by me
#include "Population.hpp"
#include "Obstacle.hpp"
//

// Variables
const int WINDOW_WIDTH = 500;
const int WINDOW_HEIGHT = 500;
const float MAX_SPEED = 5;
const float RADIUS = 3;
int POPULATION_COUNT;

float FRAMERATE = 1.0f;
bool paused = true;
bool update = true;
Population population;
std::vector<Obstacle> obstacles;
Vector2 goal;

// Functions
void reshape(int w, int h);
void draw_circle(float r, float g, float b, float x, float y, float radius);
void display();
void keyboard(unsigned char c, int x, int y);
void timer(int state); 

/*
	Entry point of the program. Everything starts to run here
 */
int main(int argc, char **argv)
{
	srand(time(NULL)); // Randomize seed based on current time
	
	// Init entities
	if (argc == 2)
		POPULATION_COUNT = std::atoi(argv[1]); // Set population count in terminal
	else
		POPULATION_COUNT = 100; // If not set just do 100

	population = Population(POPULATION_COUNT); // Create the population
	obstacles.push_back(Obstacle(-500.0f, 0.0f, 850.0f, 10.0f)); // Create can obstacle
	goal = Vector2(0.0f, 450.0);
	//

	glutInit(&argc, argv); // Init the FreeGLUT library
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // Set the graphics mode to double buffering and allow red, green, blue colour

	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT); // Set the window size to the ones above
	glutCreateWindow("Test"); // Create window with title name "Test"

	glutReshapeFunc(reshape); // Set the windows reshape function to "reshape"
	glutDisplayFunc(display); // Set main render functiont to "display"
	glutKeyboardFunc(keyboard); // Set keyboard controls
	glutTimerFunc(FRAMERATE, timer, 0); // Set main game timer loop with the framerate set

	glutMainLoop(); // Run the glut loop

	return 0;
}

void reshape(int w, int h)
{
	if (h == 0) h = 1;

	GLfloat aspect = (GLfloat)w / (GLfloat)h;

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-WINDOW_WIDTH * aspect, WINDOW_WIDTH * aspect, -WINDOW_HEIGHT, WINDOW_HEIGHT, 0.0f, 1.0f);
}

void draw_circle(float r, float g, float b, float x, float y, float radius)
{
	glBegin(GL_POLYGON);
	glColor3f(r, g, b);
	for (int i = 0; i < 360; i += 30)
		glVertex2f((radius * cos(i * M_PI / 180)) + x, (radius * sin(i * M_PI / 180)) + y);
	glEnd();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // Clear to the background to black

	// Draw each dot in population
	for (auto it = population.dots.begin(); it != population.dots.end(); ++it)
	{
		float r, g, b;

		// Colour dot
		if (it->dead) {
			r = 1.0f; g = 0.0f; b = 0.0f;
		} else if (it->successful) {
			r = 0.0f; g = 1.0f; b = 0.0f;
		} else {
			r = 1.0f; g = 1.0f; b = 1.0f;
		}

		if (it - population.dots.begin() == population.bestDot) {
			r = 0.0f; g = 1.0f; b = 1.0f;
		}

		// Draw dot
		draw_circle(r, g, b, it->pos.x, it->pos.y, RADIUS);
	}

	// Draw obstacles
	for (auto it = obstacles.begin(); it != obstacles.end(); ++it)
	{
		// Draw a rectangle that is blue
		glBegin(GL_POLYGON);
			glColor3f(0.0f, 0.0f, 1.0f);
				glVertex2f(it->pos.x, it->pos.y);
				glVertex2f(it->pos.x + it->width, it->pos.y);
				glVertex2f(it->pos.x + it->width, it->pos.y - it->height);
				glVertex2f(it->pos.x, it->pos.y - it->height);
		glEnd();
	}

	// Draw goal
	draw_circle(1.0f, 1.0f, 0.0f, goal.x, goal.y, 10.0f);

	glutSwapBuffers(); // Update screen by swaping buffer (due to double buffering)
}

void keyboard(unsigned char c, int x, int y)
{
	switch(c)
	{
		case 'u':
			update = !update;
			break;
		case 'p':
			paused = !paused;
			break;
		case '=':
			FRAMERATE += 1;
			break;
		case 'r':
			population = Population(POPULATION_COUNT);
			break;
		case '-':
			if (FRAMERATE > 1)
				FRAMERATE -= 1;
			break;
		case '+':
			FRAMERATE += 0.1f;
			break;
		case '_':
			if (FRAMERATE > 0.1f)
				FRAMERATE -= 0.1f;
			break;
	}

	std::cout << "FRAMERATE: " << FRAMERATE << std::endl;
	std::cout << "Draw: " << update << std::endl;
	std::cout << "Paused: " << paused << std::endl;
}

void timer(int state)
{
	if (!paused)
	{
		// Simulate
		if (!population.isDead())
		{
			for (auto it = population.dots.begin(); it != population.dots.end(); ++it)
			{
				// If not dead, move
				if (!it->dead && !it->successful)
				{
					// If brain has more directions
					if (it->brain.step < population.maxStep)
					{
						// Add current direction
						it->vel += it->brain.directions[it->brain.step];
						// Increment brain step index
						it->brain.step++;

						// Speed limit
						if (it->vel.x > MAX_SPEED) {
							it->vel.x = MAX_SPEED;
						} else if (it->vel.x < -MAX_SPEED) {
							it->vel.x = -MAX_SPEED;
						}
						if (it->vel.y > MAX_SPEED) {
							it->vel.y = MAX_SPEED;
						} else if (it->vel.y < -MAX_SPEED) {
							it->vel.y = -MAX_SPEED;
						}

						// Move
						it->pos += it->vel;

						// Reached goal
						if (it->pos.x + RADIUS >= goal.x - RADIUS &&
							it->pos.x - RADIUS <= goal.x + RADIUS &&
							it->pos.y + RADIUS >= goal.y - RADIUS &&
							it->pos.y - RADIUS <= goal.y + RADIUS)
						{
							it->successful = true;
						}
						
						// Touched side of screen
						if (it->pos.x > WINDOW_WIDTH ||
							it->pos.x < -WINDOW_WIDTH ||
							it->pos.y > WINDOW_HEIGHT ||
							it->pos.y < -WINDOW_HEIGHT)
						{
							it->dead = true;
						}

						for (auto jt = obstacles.begin(); jt != obstacles.end(); ++jt)
						{
							// Touched obstacle
							if (it->pos.x + RADIUS >= jt->pos.x - jt->width &&
								it->pos.x - RADIUS <= jt->pos.x + jt->width &&
								it->pos.y + RADIUS >= jt->pos.y - jt->height &&
								it->pos.y - RADIUS <= jt->pos.y + jt->height)
							{
								it->dead = true;
							}
						}
					}
					// Out of steps
					else
					{
						it->dead = true;
					}
				}
			}
		}
		// New generation
		else
		{
			population.calculateFitnessSum(goal);
			population.nextGeneration();
			population.mutate();
			std::cout << "Max Steps: " << population.maxStep << std::endl;
			std::cout << "Generation: " << population.generation << std::endl;
		}
	}

	// Update screen
	if (update)
		glutPostRedisplay();

	glutTimerFunc(FRAMERATE, timer, 0);
}