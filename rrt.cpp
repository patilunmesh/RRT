#include <iostream>
#include <stdlib.h>
#include <vector>
#include <math.h>
using namespace  std;

// section 1 parameters and structures
//params in cm or degree
	double GRID_RESOLUTION = 0.1;
	int GRID_WIDTH = 10;
	int GRID_HEIGHT = 10;
	int STEER_LIMIT_LEFT = 45;
	int STEER_LIMIT_RIGHT = 45;
	int ITERATIONS = 100;

// structs

	struct Node
	{
		int x, y;
		int parent_id;
		int u;
	};

vector<Node> nodeList; // vector of nodes
nodeList.push_back(Node()); // pushing new node created with default constructor


int randomThetaGenerator();
Node nodePushMerge();
int randomX();
int randomY();

int main()
{
	

	//section2 init vars #####################################################################
	int theta; //heading within steer limits right is negative left is positive
	Node init; // initial pose
	Node goal; //goal pose
	int jumpDistance = GRID_RESOLUTION;

	//input section (obstacles, start pose, goal pose, resolution, grid size)###################
	int x, y, parent_id, u;
	cout << "enter start pose: x, y";
	cin >> x >> y ;
	init = nodePushMerge(x, y, 0, 0);

	cout << "enter goal pose: x, y";
	cin >> x >> y ;
	goal = nodePushMerge(x, y,0, 0);
	
	nodeList.push_back(init);
	//nodeList.push_back(goal);

	//cout << "init and goal is (" << init.x << " " << init.y << ") (" << goal.x << " " << goal.y << ")\n";

	//section 3 call in loop random generator ##################################################

	for (int i = 0; i < ITERATIONS; i++)
	{
		x_new = randomX();
		y_new = randomY();
		near_index = getNearestNode(x_new, y_new);
		x_old = nodeList[near_index].x
		y_old = nodeList[near_index].y
		bCollision = checkCollision(x_new, y_new, x_old,y_old )
		if (!bCollision)
		{
			D = getDistance(x_new, y_new, x_old, y_old);
			u = getAngle(x_new, y_new, x_old, y_old);
			if (D > jumpDistance)
			{
				populateNodes(x_new, y_new, x_old, y_old, u, D);
			}
			if (D <= jumpDistance)
			{
				ID++;
				Node m = nodePushMerge(x_new, y_new, near_index, u);
			}
			DGoalCheck = getDistance(x_new, y_new, goal.x, goal.y);
			if (DGoalCheck <= jumpDistance)
			{
				cout << "success goal reached" << endl;
				pathGenerator(near_index);
			}
		}
		else {ITERATIONS ++ ;}
		

	}

	//path smoothing #########################################################################

	return 0;
}

// random theta generator
int randomThetaGenerator()
{
	int i = rand() % 10;
	int theta_new = (STEER_LIMIT_RIGHT + STEER_LIMIT_LEFT)*(i-1)/9 - STEER_LIMIT_RIGHT;
	//return theta_new;	
}

int randomX ()
{
	int x = rand() % GRID_HEIGHT;
	return x;
}

int randomY ()
{
	int y = rand() % GRID_WIDTH;
	return y;
}
// collision checker



//add node, add edge
Node nodePushMerge(int x, int y, int id, int parent_id, int u)
{
	Node n;
	n.x = x;
	n.y = y;
	n.id = id;
	n.parent_id = parent_id;
	n.u = u;
	return n;
}


//path store



//path smoothing