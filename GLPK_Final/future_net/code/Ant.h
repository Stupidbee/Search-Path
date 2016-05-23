#ifndef _ANT_H
#define _ANT_H

#include <iostream>
#include <vector>
#include <cstdlib>

using std::vector;

#define INF 65535
#define POINT_NUM 600

extern const double DBQ;
extern const double ROU;
extern const double DB_MAX;

extern double g_cost[POINT_NUM][POINT_NUM];
extern double g_trail[POINT_NUM][POINT_NUM];
extern double g_prob[POINT_NUM][POINT_NUM];
extern double g_distance_beta[POINT_NUM][POINT_NUM];
extern vector<int> out_point[POINT_NUM];
extern vector<int> demand;
extern bool g_solved;
extern int start_point;
extern int end_point;
extern int max_ver;
extern int ant_num;
extern int iter_num;

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