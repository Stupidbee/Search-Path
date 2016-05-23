#ifndef _DPSP_H
#define _DPSP_H

#include "common.h"
#include "Ant.h"

class DPSP
{
public:
	Ant *ant_array;
	Ant ant_best;
	double ** trail;
	int *subset_flag;
	set<int> demand;

public:
	DPSP(set<int> &d);
	~DPSP();
	void init_data();
	void search();
	void update_trail();
	bool report(vector<int> &result, vector<int> &point);
};

#endif