#include <iostream>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include <chrono>
#include <random>

using namespace  std;

// section 1 parameters and structures
//params in cm or degree
	int GRID_RESOLUTION = 1;
	int GRID_WIDTH = 10;
	int GRID_HEIGHT = GRID_WIDTH;
	int STEER_LIMIT_LEFT = 180;
	int STEER_LIMIT_RIGHT = -180;
	int ITERATIONS = 1000;

// structs

	struct Node
	{
		float x, y;
		int parent_id;
		float u;
	};
	struct Point
	{
		float x;
		float y;
	};

vector<Node> nodeList; // vector of nodes

// function templates 
	Node nodePushMerge(float x, float y, int parent_id, float u);
	Point randomXY();
	//int randomY();
	float getSQDistance(float x1, float y1, float x2, float y2);
	float getAngle (float x1, float y1, float x2, float y2);
	bool checkCollision(float x, float y, float x_old, float y_old);
	int getNearestNode(float x_new, float y_new);
	void pathGenerator(int index);
	int populateNodesCheckGoal(float x_new, float y_new, float xg, float yg, int near_index, float u, float Dsq);

int main()
{
	//section2 init vars #####################################################################
	Node init; // initial pose
	Node goal; //goal pose

	//input section (obstacles, start pose, goal pose, resolution, grid size)###################
	float x, y;
	int parent_id;
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
	
	int near_index, reachIndex;
	float x_new, y_new, x_old, y_old, Dsq;
	bool bCollision;
	bool bGoal = false ;
	srand((unsigned) time(0));
	for (int i = 0; i < ITERATIONS; i++)
	{
		Point p = randomXY();
		x_new = p.x;
		y_new = p.y;
		near_index = getNearestNode(x_new, y_new);
		x_old = nodeList[near_index].x ;
		y_old = nodeList[near_index].y ;
		//cout << "old  x " << x_old << " y "<< y_old << endl;
		u = getAngle(x_new, y_new, x_old, y_old);
		//cout << "angle " << u << endl;
		if (x_new == x_old || y_new == y_old)
		{
			ITERATIONS ++;
			continue;
		}
		/*if (not((u >= 0 && u <= STEER_LIMIT_LEFT ) || (u < 0 && u > STEER_LIMIT_RIGHT)))
		{
			ITERATIONS++ ; 
			continue;
		}*/ //for steer limit restrictions
		bCollision = checkCollision(x_new, y_new, x_old, y_old);
		if (!bCollision)
		{
			Dsq = getSQDistance(x_new, y_new, x_old, y_old);
			//cout << " DSQ " << Dsq << " res "<< GRID_RESOLUTION << endl;
			if (Dsq > GRID_RESOLUTION)
			{
				reachIndex = populateNodesCheckGoal(x_new, y_new, goal.x, goal.y, near_index, u, Dsq); // also check if goal reached
				bGoal = (reachIndex != -1)? true : false;
				//cout << "check post 1 \n";
			}
			if (Dsq <= GRID_RESOLUTION)
			{
				//cout << "check post 2 \n";
				Node m = nodePushMerge(x_new, y_new, near_index, u);
				::nodeList.push_back(m);
				float DGoalChecksq = getSQDistance(x_new, y_new, goal.x, goal.y);
				if (DGoalChecksq <= GRID_RESOLUTION)
				{
					//cout << "check post 3 \n";
					bGoal = true;
					reachIndex = nodeList.size() - 1;
				}
			}
		}
		else {
			ITERATIONS ++ ;
			continue;
		}
		cout << "number of iterations " << i + 1 << endl;
		if (bGoal)
		{
			cout << "success goal reached" << endl;
			if(nodeList[reachIndex].x != goal.x)
			{
				goal = nodePushMerge(goal.x , goal.y, reachIndex, 0.0f);
				nodeList.push_back(goal);
				pathGenerator(nodeList.size() - 1);	
			}
			else {pathGenerator(reachIndex);}
			
			break;
		}
	}
	//cout << "size "<< nodeList.size() << endl;
		
	/*
	cout << "whole tree \n\n";
	cout << "x  " << " y "<< endl;
	for (int i = 0; i < nodeList.size(); i++)
	{
		cout << nodeList[i].x << "  " << nodeList[i].y << endl;
		
	}
	*/ // to print whole tree if needed
	return 0;
}

// helping functions
float getSQDistance(float x1, float y1, float x2, float y2)
{
	return (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1);
}

float getAngle (float x1, float y1, float x2, float y2)
{
	float t = atan2(y2 - y1, x2 - x1);
	return t*180/3.1416f; 
}

// random generator
Point randomXY()
{
	Point p;
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> distributionInteger(0, GRID_WIDTH);
    // random engine setup complete
    p.x = distributionInteger(generator);
	p.y = distributionInteger(generator);
	return p;	
}

// collision checker

bool checkCollision(float x, float y, float x_old, float y_old)
{
	return false;
}
//add node, add edge
Node nodePushMerge(float x, float y, int parent_id, float u)
{
	Node n;
	n.x = x;
	n.y = y;
	n.parent_id = parent_id;
	n.u = u;
	return n;
}

int getNearestNode (float x_new, float y_new) //euclidean checking
{
	int near_index = 0;
	int size = nodeList.size();
	int minD = GRID_HEIGHT*GRID_HEIGHT + GRID_WIDTH*GRID_WIDTH;
	for (int i = 0; i < size; i++)
	{
		float D = getSQDistance(x_new, y_new, nodeList[i].x, nodeList[i].y);
		if (D < minD)
		{
			minD = D;
			near_index = i;
		}
	}
	//cout << "near " << near_index<< endl;
	return near_index;
}

int populateNodesCheckGoal(float x_new, float y_new, float xg, float yg, int near_index, float u, float Dsq)
{
	float x_, y_;
	int pid;
	float x_near = nodeList[near_index].x;
	float y_near = nodeList[near_index].y;
	//cout << x_near << " " << y_near << " x y" << endl;
	int iter = sqrt(Dsq) / GRID_RESOLUTION;
	float gapx = abs(x_new - x_near);
	float xs = (x_new > x_near)? 1 : -1; 
	float gapy = abs(y_new - y_near);
	float ys = (y_new > y_near)? 1 : -1; 

	for (int i = 1; i <= iter; i++)
	{
		x_ = x_near + (gapx/iter) * i * xs;
		y_ = y_near + (gapy/iter) * i * ys;
		pid = near_index;
		Node n = nodePushMerge(x_, y_, pid, u); 
		nodeList.push_back(n);
		//cout << i << " "<< x_ << " "<< y_ << " "<< x_new << " " << y_new <<" pushed \n";

		near_index = nodeList.size() - 1;

		float DGoalChecksq = getSQDistance(x_ , y_, xg, yg);
		
		if (DGoalChecksq < GRID_RESOLUTION)
		{
			return near_index;
			break;
		}
	}
	return -1;
}
//path store

void pathGenerator(int index)
{
	vector<Node> path;
	cout << "x  " << " y "<< endl;
	while (index != 0)
	{
		path.push_back(nodeList[index]); 
		cout << nodeList[index].x << "  " << nodeList[index].y << endl;
		index = nodeList[index].parent_id;
	}
	
	cout << "path generated\n";

}

//path smoothing