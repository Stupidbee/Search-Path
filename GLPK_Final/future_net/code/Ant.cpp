#include <cstring>
#include <iostream>
#include "Ant.h"

using namespace std;

Ant::Ant()
{

}

Ant::~Ant()
{

}

void Ant::init()
{
	path.clear();

	memset(allowed_point, 0, sizeof(int)*POINT_NUM);
	memset(include_point, 0, sizeof(int)*POINT_NUM);

	current_point = start_point;
	path.push_back(current_point);
	allowed_point[current_point] = 1;
	path_dist = 0.0;
	complete_flag = false;
	contain_flag = false; 
}

int Ant::next_point()
{
	int next = -1;
	int count = 0;
	double total = 0.0;
	double prob[600] = {-1};
//	memset(prob, -1.0, 600);
	int *next_point_array = new int[600];
	int index = -1;
	int next_way;

	for(int i=0; i<out_point[current_point].size(); i++)
	{
		next_way = out_point[current_point][i];
		if(allowed_point[next_way] != 1)
		{
			prob[count] = g_prob[current_point][next_way];
			total += prob[count];
			next_point_array[count] = next_way;
			count++;
		}
	}
//	cout << total << endl;
	if(count == 0)
	{
		delete [] next_point_array;
		return next;
	}

	index = -1;
	int first = -1;
	double temp = rnd(0.0, total);
	for(int i=0; i<count; i++)
	{
		if(prob[i] >= 0)
		{
			temp -= prob[i];
			if(temp <= 0.0)
			{
				index = i;
				break;
			}
		}
		if(first == -1)
		{
			first = i;
		}
	}

	if(index == -1)
	{
		index = first;
	}

	next = next_point_array[index];
	delete [] next_point_array;
	return next;

}

void Ant::move()
{
	if(complete_flag == true)
	{
		return;
	}

	int point = next_point();

	if(point < 0)
	{
		int count = path.size();
		if(count >= 2)
		{
			current_point = path[count-2];
			path.pop_back();
		}
		else
		{
			g_solved = false;
		}
	}
	else
	{
		g_prob[current_point][point] *= 1;
		path.push_back(point);
		allowed_point[point] = 1;
		current_point = point;

		if(current_point == end_point)
		{
//			cout << "daoda" << endl;
			complete_flag = true;
			caculate_dist();
		}
	}
}

void Ant::caculate_dist()
{
	path_dist = 0;
	int count = 0;
	for(vector<int>::iterator it=path.begin(); it!=path.end()-1; ++it)
	{
//		cout << *(it+1) << " ";
		path_dist += g_cost[*it][*(it+1)];
		include_point[*(it+1)] = 1;
	}
//	cout << endl;
	for(vector<int>::iterator it=demand.begin(); it!=demand.end(); ++it)
	{
		if(include_point[*it] == 1)
		{
//			cout << *it << " ";
			count++;
		}
		
	}	
//	cout << "count" << count << endl;
	if(count == demand.size())
	{		
		contain_flag = true;
	}
}