#include <iostream>
#include <stdlib.h>
using namespace  std;

// section 1 parameters
	double GRID_RESOLUTION = 1;
	int GRID_WIDTH = 10;
	int GRID_HEIGHT = 10;
	int STEER_LIMIT_LEFT = 45;
	int STEER_LIMIT_RIGHT = 45;

int random_generator();

int main()
{
	

	//section2 init vars
	int theta; //heading within steer limits right is negative left is positive

	//input section (obstacles, start pose, goal pose, resolution, grid size)

	// section 3 call in loop random generator
	for (int i = 0; i < 10; ++i)
	{
		theta = random_generator();
		cout << theta <<endl;
	}
	

	// section 4 validation n linking

	// section 5 add vertex add node

	// goal check

	//path smoothing

	return 0;
}

// random theta generator
int random_generator()
{
	double i = rand() % 10;
	int theta_new = (STEER_LIMIT_RIGHT + STEER_LIMIT_LEFT)*(i-1)/9 - STEER_LIMIT_RIGHT;
	return theta_new;
}
	
// collision checker



//add node, add edge



//path store



//path smoothing