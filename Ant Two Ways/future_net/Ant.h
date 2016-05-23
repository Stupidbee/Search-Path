#ifndef _ANT_H
#define _ANT_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include "common.h"

using std::vector;

inline double rnd(double dbLow,double dbUpper)
{
	double dbTemp=rand()/(double)RAND_MAX;
	return dbLow+dbTemp*(dbUpper-dbLow);
}

class Ant
{
public:
	Ant();
	~Ant();
	int next_point();
	void init();
	void move();
	void caculate_dist();

public:	
	vector<int> path;
	double path_dist;

	int allowed_point[POINT_NUM];
	int include_point[POINT_NUM];
	int current_point;	

	bool complete_flag;
	bool contain_flag;

};

#endif