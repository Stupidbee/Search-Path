#include <ctime>
#include <cmath>
#include <iostream>
#include <vector>
#include <set>
#include <fstream>
#include "GeneAlg.h"

using std::vector;
using std::set;
using std::ofstream;

int a = 0;
extern const char *result_name;

void write_file(const char *name, vector<int> &point, const Graph &G);

GeneAlg::GeneAlg(Graph g, vector<int> s, int st, int e):G(g),subset(s),start(st),end(e)
{
	population = new genetype[POP_SIZE];
	new_population = new genetype[POP_SIZE];
	for (int i=0; i<POP_SIZE; i++)
	{
		population[i].course = 0;
		population[i].path_fit = 0.0;
		population[i].path_rfit = 0.0;
		population[i].point_fit = 0.0;
		population[i].point_rfit = 0.0;
		population[i].rfit = 0.0;
		population[i].fit = 0.0;
		population[i].cfit = 0.0;
		population[i].point_num = 0;
	}
#if 0
	set_extend = subset;
	int n;
	for(int i=0; i<10; i++)
	{
		n = rand() % G.vertex_num;
		if(find(set_extend.begin(), set_extend.end(), G.vertex[n]) == set_extend.end()
			&& G.vertex[n] != start && G.vertex[n] != end)
		{
			set_extend.push_back(G.vertex[n]);
		}
	}
#endif
	better_point.course = 65535;
	better_point.point_num = 0;
	better.fit = 0.0;
	sum_fit = 0.0;
	generation = 0;
}

GeneAlg::~GeneAlg()
{
	delete [] population;
	delete [] new_population;
}

void GeneAlg::initalize()
{
	srand((unsigned)time(NULL));
	int n = 0;
	while(n < POP_SIZE)
	{
		if(subset.size() < 11)
		{
			if(G.search_path_random(start, end, population[n].gene))
			{
				n++;
			}
		}
		else
		{
			if(product_gene(population[n].gene))
			{
				n++;
			}
		}
	}
}

void GeneAlg::select()
{
	double p;
	for (int i = 0; i < POP_SIZE; i++)
    { 
		p = rand()%10000/10000.0;
		if (p < population[0].cfit)
		{
			new_population[i] = population[0];      
		}
		//轮盘赌选择种群个体
		else
		{
			for (int j = 0; j <POP_SIZE; j++)
			{			     
				if (p >= population[j].cfit 
					&& p < population[j+1].cfit)
                {
//                	cout << p << " " << j+1 << endl;
                	new_population[i] = population[j+1];
                } 
            } 
		}
	}
	//更新当前种群
	for (int i=0; i<POP_SIZE; i++)
	{
		population[i] = new_population[i];
	}
}

void GeneAlg::xovercross()
{
	double x;
	int n = 0;
	int a;
	int b;
	while(n < 80)
	{
		x=rand()%1000/1000.0;
		if (x < PXOVER)
		{			
			a = rand()%POP_SIZE;
			b = rand()%POP_SIZE;
			if(a == b)
			{
				continue;
			}
			else
			{
				if(xover(a, b))
				{
					xover2(a, b);
				}
			}
		}
		n++;	
	}
}

void GeneAlg::pop_mutate()
{
	double x;
	for (int i=0; i<POP_SIZE; i++)
	{
		x=rand()%1000/1000.0;
		if (x < PMUTATION)
		{
			if(!mutate(population[i].gene))
			{
				mutate_random(population[i].gene);
			}
		}
	}
}

void GeneAlg::iterative(vector<int> &best, int &best_dist)
{
//	ofstream os("result.txt");
	initalize();
	evaluate();
	keep_the_point_best();
	while (generation <  MAX_GENE)
	{
		select();
		keep_the_point_best();	
		xovercross();		
		pop_mutate();
		evaluate();
		generation++;
		best = better_point.gene;
		best_dist = better_point.course;
		if(clock()*1000/CLOCKS_PER_SEC > 9000)
		{			
			break;
		}
	}
}

void GeneAlg::report()
{
	write_file(result_name, better_point.gene, G);
}


