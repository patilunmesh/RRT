#include <iostream>
#include <stdlib.h>
#include <vector>
#include <math.h>
using namespace  std;

// section 1 parameters and structures
//params in cm or degree
	int GRID_RESOLUTION = 1;
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
		float u;
	};

vector<Node> nodeList; // vector of nodes
//nodeList.push_back(Node()); // pushing new node created with default constructor

Node nodePushMerge(int x, int y, int parent_id, float u);
int randomX();
int randomY();
int getSQDistance(int x1, int y1, int x2, int y2);
float getAngle (int x1, int y1, int x2, int y2);
bool checkCollision(int x, int y, int x_old, int y_old);
int getNearestNode(int x_new, int y_new);
void pathGenerator(int index);
int populateNodesCheckGoal(int x_new, int y_new, int xg, int yg, int near_index, float u, int Dsq);

int main()
{
	

	//section2 init vars #####################################################################
	int theta; //heading within steer limits right is negative left is positive
	Node init; // initial pose
	Node goal; //goal pose

	//input section (obstacles, start pose, goal pose, resolution, grid size)###################
	int x, y, parent_id;
	float u;
	cout << "enter start pose: x, y";
	cin >> x >> y ;
	init = nodePushMerge(x, y, 0, 0.0f);

	cout << "enter goal pose: x, y";
	cin >> x >> y ;
	goal = nodePushMerge(x, y, 0, 0.0f);
	
	::nodeList.push_back(init);

	cout << "init and goal is (" << init.x << " " << init.y << ") (" << goal.x << " " << goal.y << ")\n";

	//section 3 call in loop random generator ##################################################
	int near_index, x_new, y_new, x_old, y_old, Dsq,reachIndex;

	bool bCollision;
	bool bGoal = false ;
	for (int i = 0; i < ITERATIONS; i++)
	{
		x_new = randomX();
		y_new = randomY();
		near_index = getNearestNode(x_new, y_new);
		//cout << "near " << near_index << endl;
		u = getAngle(x_new, y_new, x_old, y_old);
		if ((u < 0 && u > STEER_LIMIT_LEFT ) || (u >= 0 && u <= STEER_LIMIT_RIGHT))
		{
			ITERATIONS++ ; 
			continue;
		}
		x_old = nodeList[near_index].x ;
		y_old = nodeList[near_index].y ;
		bCollision = checkCollision(x_new, y_new, x_old, y_old);
		if (!bCollision)
		{
			Dsq = getSQDistance(x_new, y_new, x_old, y_old);
			cout << " DSQ " << Dsq << " res "<< GRID_RESOLUTION << endl;
			if (Dsq > GRID_RESOLUTION)
			{
				reachIndex = populateNodesCheckGoal(x_new, y_new, goal.x, goal.y, near_index, u, Dsq); // also check if goal reached
				bGoal = (reachIndex != -1)? true : false;
				cout << "check post 1 \n";
			}
			if (Dsq <= GRID_RESOLUTION)
			{
				cout << "check post 2 \n";
				Node m = nodePushMerge(x_new, y_new, near_index, u);
				::nodeList.push_back(m);
				int DGoalChecksq = getSQDistance(x_new, y_new, goal.x, goal.y);
				if (DGoalChecksq <= GRID_RESOLUTION)
				{
					cout << "check post 3 \n";
					bGoal = true;
					reachIndex = nodeList.size() - 1;
				}
			}
		}
		else {
			ITERATIONS ++ ;
			continue;
		}
		cout << "number of iter " << i + 1 << endl;
		if (bGoal)
		{
			cout << "success goal reached" << endl;
			pathGenerator(reachIndex);
			break;
		}

	}

	//path smoothing #########################################################################

	return 0;
}

// helping functions
int getSQDistance(int x1, int y1, int x2, int y2)
{
	return (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1);
}

float getAngle (int x1, int y1, int x2, int y2)
{
	float t = atan2(y2 - y1, x2 - x1);
	return t*180/3.1416f; 
}

// random generator
int randomX ()
{
	int x = rand() % GRID_HEIGHT;
	cout << " . " << x << endl;
	return x;
}

int randomY ()
{
	int y = rand() % GRID_WIDTH;
	return y;
}
// collision checker

bool checkCollision(int x, int y, int x_old, int y_old)
{
	return false;
}
//add node, add edge
Node nodePushMerge(int x, int y, int parent_id, float u)
{
	Node n;
	n.x = x;
	n.y = y;
	n.parent_id = parent_id;
	n.u = u;
	return n;
}

int getNearestNode (int x_new, int y_new)
{
	int near_index = 0;
	int size = nodeList.size();
	int minD = GRID_HEIGHT*GRID_HEIGHT + GRID_WIDTH*GRID_WIDTH;
	for (int i = 0; i < size; i++)
	{
		int D = getSQDistance(x_new, y_new, nodeList[i].x, nodeList[i].y);
		if (D < minD)
		{
			minD = D;
			near_index = i;
		}
	}
	return near_index;
}

int populateNodesCheckGoal(int x_new, int y_new, int xg, int yg, int near_index, float u, int Dsq)
{
	cout << "check post in populate \n";
	int x_, y_, pid;
	int x_near = nodeList[near_index].x;
	int y_near = nodeList[near_index].y;
	while(Dsq > GRID_RESOLUTION)
	{
		cout << x_near << " " << y_near << " x y" << endl;
		x_ = x_near + GRID_RESOLUTION*cos(u);
		y_ = y_near + GRID_RESOLUTION*sin(u);
		Dsq = getSQDistance(x_, y_, x_new, y_new);
		pid = near_index;
		Node n = nodePushMerge(x_, y_, pid, u); 
		nodeList.push_back(n);
		cout << Dsq << " "<< x_ << " "<< y_ << " "<< x_new << " " << y_new <<" pushed \n";

		near_index = nodeList.size() - 1;

		int DGoalChecksq = getSQDistance(x_ , y_, xg, yg);
		
		if (DGoalChecksq < GRID_RESOLUTION)
		{
			return near_index;
			break;
		}
		x_near = x_ ;
		y_near = y_ ;

	}
	cout << "returning \n";
	return -1;
}
//path store

void pathGenerator(int index)
{
	vector<Node> path;
	while (index != 0)
	{
		path.push_back(nodeList[index]);
		
		cout << "x " << nodeList[index].x << " y " << nodeList[index].y << endl;
		index = nodeList[index].parent_id;
	}
	cout << "path generated\n";

}

//path smoothing