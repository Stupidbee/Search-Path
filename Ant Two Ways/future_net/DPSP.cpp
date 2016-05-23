#include "DPSP.h"
#include <cstring>

DPSP::DPSP(set<int> &d):demand(d)
{
	ant_best.path_dist = DB_MAX;
	trail = new double *[POINT_NUM];
	for (int i=0; i<POINT_NUM; i++)
	{
		trail[i] = new double[POINT_NUM];
	}
	ant_array = new Ant[ant_num];
	subset_flag = new int[POINT_NUM];
}

DPSP::~DPSP()
{
	for(int i=0;i<POINT_NUM;i++)
	{
		delete [] trail[i];
	}
	delete [] trail;
	delete [] ant_array;
	delete [] subset_flag;
}

void DPSP::init_data()
{
	memset(subset_flag, 0, sizeof(int)*(max_ver+1));
	for(set<int>::iterator it=demand.begin(); it!=demand.end(); ++it)
	{
		subset_flag[*it] = 1;
//		cout << *it << endl;
	}
	for(int i=0; i<max_ver+1; i++)
	{
		for(int j=0; j<max_ver+1; j++)
		{
			g_trail[i][j] = DBQ;
			if (i != j)
			{
				g_distance_beta[i][j] = 1 / (g_cost[i][j] * g_cost[i][j]);
			}

			if(subset_flag[j] == 1)
			{
				g_prob[i][j] = g_trail[i][j];
			}
			else
			{
				g_prob[i][j] = g_trail[i][j] * g_distance_beta[i][j];
			}
		}
	}
	g_solved = true;
}

void DPSP::search()
{
	int count = 0;
	int iter = 0;
	while(iter < iter_num || ant_best.path.size() == 0)
	{
		iter ++;
//		cout << iter << endl;
		for(int i=0; i<ant_num; i++)
		{
			ant_array[i].init();
		}

		while(true)
		{
			count = 0;
			for(int i=0; i<ant_num; i++)
			{
				ant_array[i].move();
				if(ant_array[i].complete_flag == true)
				{
					count++;
				}
			}

			if (count == ant_num)
			{
				break;
			}

			if (g_solved == false)
			{
				break;
			}
		}
		for(int i=0; i<ant_num; i++)
		{
			if(ant_array[i].contain_flag == true)
			{
				if(ant_array[i].path_dist < ant_best.path_dist)
				{
					ant_best.path_dist = ant_array[i].path_dist;
					ant_best.path = ant_array[i].path;
				}
				ant_array[i].contain_flag = false;
			}
		}
		update_trail();
	}
}

void DPSP::update_trail()
{
	int count = 0;
	count = ant_best.path.size();
	if(count != 0)
	{
		for (int i=0; i<max_ver+1; i++)
		{
			for(int j=0; j<max_ver+1; j++)
			{
				trail[i][j] = 0.0;
			}
		}
		int m = 0;
		int n = 0;

		double dist_temp = 1.0/(ant_best.path_dist);

		for(int i=1; i<count; i++)
		{
			m = ant_best.path[i];
			n = ant_best.path[i-1];
			trail[n][m] += dist_temp; 
		}

		for(int i=0; i<max_ver+1; i++)
		{
			for(int j=0; j<max_ver+1; j++)
			{
				g_trail[i][j] = g_trail[i][j]*ROU + trail[i][j];
			}
		}

		for(int i=0; i<max_ver+1; i++)
		{
			for(int j=0; j<max_ver+1; j++)
			{
				g_prob[i][j] = g_trail[i][j] * g_distance_beta[i][j];
			}
		}

	}
}

bool DPSP::report(vector<int> &result, vector<int> &point)
{
	result.clear();
	point.clear();
	if(!ant_best.path.empty())
	{
		point = ant_best.path;
		for(vector<int>::iterator it=ant_best.path.begin(); it!=ant_best.path.end()-1; ++it)
		{
			result.push_back(id[*it][*(it+1)]);	
		}
		return true;
	}	
	else
	{
		return false;
	}
}
