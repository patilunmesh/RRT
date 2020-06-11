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
	int jumpDSQ = GRID_RESOLUTION **2 ;

	//input section (obstacles, start pose, goal pose, resolution, grid size)###################
	int x, y, parent_id, u;
	cout << "enter start pose: x, y";
	cin >> x >> y ;
	init = nodePushMerge(x, y, 0, 0);

	cout << "enter goal pose: x, y";
	cin >> x >> y ;
	goal = nodePushMerge(x, y,0, 0);
	
	::nodeList.push_back(init);
	//nodeList.push_back(goal);

	//cout << "init and goal is (" << init.x << " " << init.y << ") (" << goal.x << " " << goal.y << ")\n";

	//section 3 call in loop random generator ##################################################
	int near_index, x_new, y_new, x_old, y_old, Dsq, jumpDSQ;
	bool bCollision;
	bool bGoal = false ;
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
			Dsq = getSQDistance(x_new, y_new, x_old, y_old);
			u = getAngle(x_new, y_new, x_old, y_old);
			if (Dsq > jumpDSQ)
			{
				bGoal = populateNodesCheckGoal(x_new, y_new, x_old, y_old, u, Dsq); // also check if goal reached
			}
			if (Dsq <= jumpDSQ)
			{
				Node m = nodePushMerge(x_new, y_new, near_index, u);
				::nodeList.push_back(m);
			}

			DGoalChecksq = getSQDistance(x_new, y_new, goal.x, goal.y);
			if (DGoalChecksq <= jumpDSQ)
			{
				bGoal = true;
			}
		}
		else {ITERATIONS ++ ;}
		
		if (bGoal)
		{
			cout << "success goal reached" << endl;
			pathGenerator(near_index);
			break;
		}

	}

	//path smoothing #########################################################################

	return 0;
}

// helping functions
int getSQDistance(int x1, int y1, int x2, int y2)
{
	return (x2 - x1)**2 + (y2 - y1)**2;
}

int getAngle (int x1, int y1, int x2, int y2)
{
	
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

bool checkCollision(int x, int y)
{
	return false;
}

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

int getNearestNode (int x_new, int y_new)
{
	int near_index = 0;
	int size = nodeList.size()
	int minD = GRID_HEIGHT**2 + GRID_WIDTH**2;
	for (int i = 0; i < size; ++i)
	{
		x = nodeList[i].x;
		y = nodeList[i].y;
		int D = getSQDistance(x_new, y_new, x, y);
		if (D < minD)
		{
			minD = D;
			near_index = i;
		}
	}
	return near_index;
}

//path store



//path smoothing