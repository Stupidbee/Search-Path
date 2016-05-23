#ifndef _DPSP_H
#define _DPSP_H
#include "Ant.h"

#define INF 65535
#define POINT_NUM 600

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


extern const char *result_name;

void write_file(const char *name, vector<int> &point); 

class DPSP
{
public:
	Ant *ant_array;
	Ant ant_best;
	double ** trail;
	int *subset_flag;

public:
	DPSP();
	~DPSP();
	void init_data();
	void search();
	void update_trail();
	void report();
};

#endif