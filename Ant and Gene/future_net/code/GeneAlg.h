#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <utility>
#include <algorithm>
#include "Graph.h"

using std::ostream;

void print(vector<int> &, ostream &os);
extern int a;

using std::vector;
using std::set;
using std::pair;
using std::find;

class GeneAlg
{
public:
	GeneAlg(Graph G, vector<int> subset, int st, int e);
	~GeneAlg();

	void initalize();	
	void select();
	void xovercross();
	void pop_mutate();
	void iterative(vector<int> &gene, int &best_dist);
	void report();

	inline void evaluate();	
	inline void keep_the_point_best();

private:
	struct genetype
	{
		vector<int> gene;                           //染色体编码
		int course;                                 //个体代表问题解的目标值
		int point_num;                              //路径包含的子集点数
		double path_fit;                            //路径的适应度                                
		double path_rfit;                           //路径的相对的适应度	
		double point_fit;                           //相对适应度的积累概率
		double point_rfit;                          //必经点的相对适应度
		double rfit;                                //总体相对适应度
		double fit;                                 //适应度
		double cfit;                                //累积概率                 
	};
	Graph G;                                        //图的结构		
	vector<int> subset;                             //必经点集合
	int start;                                      //路径起始点
	int end;                                        //路径终点	
                                      
	genetype *population;                           //当前种群
	genetype *new_population;                       //新种群
	genetype better;                                //种群中当前最优秀的个体
	genetype better_point;                          //包含全部子集节点且处于当前路径最短的个体
	double sum_fit;                                 //种群中适应度的总和
	int generation;                                 //代数的计数器
	int same_pop_num;                               //同质个体数目
	
	static const int POP_SIZE = 25;                 //种群规模40
	static const double PXOVER = 1.0;               //交换概率
	static const double PMUTATION = 1.0;            //变异概率
	static const int MAX_GENE = 40;               //遗传最大的代数20
	static const double POINT_PER = 0.5;            //点数占比重
	static const double COURSE_PER = 0.5;           //路径长度占比重

	inline bool product_gene(vector<int> &gene);
	inline bool gene_judge(vector<int> &gene);      //判断染色体是否合法
	inline int get_gene_course(vector<int> &gene);
	inline int get_gene_point(vector<int> &gene);
	inline bool xover(int x1, int x2);
	inline bool xover2(int x1, int x2);
	inline bool mutate(vector<int> &gene);
	inline bool mutate_insert(vector<int> &gene, int p, vector<int> &s);
	inline bool mutate_random(vector<int> &gene);
	inline bool loop_judge(vector<int> &gene, int &elem);
	inline void loop_remove(vector<int> &gene); 
	inline bool same_point(vector<int> &gene1, vector<int> &gene2, vector<int> &s);
	inline void remain_point(vector<int> &gene, vector<int> &s);
	inline void include_point(vector<int> &gene, vector<int> &s);
	inline void local_degen();
};

void GeneAlg::local_degen()
{
	for(int i=0; i<POP_SIZE; i++)
	{
		if(better.gene == population[i].gene)
		{			
			if(rand()%2 == 0)
			{
				population[i].gene.clear();
				while(!product_gene(population[i].gene));
				cout << generation << endl;
				print(population[i].gene, cout);		
			}			
		}
	}
}

bool GeneAlg::product_gene(vector<int> &gene)
{
	int m = (rand() % 5)+1;
	int dist;
	set<int> s;
	vector<int> v;
	for(int i=0; i<m; i++)
	{
		int j = rand()%(subset.size());
		s.insert(j);			
	}

	for(set<int>::iterator set_iter=s.begin(); set_iter!=s.end(); set_iter++)  		 
	{
		v.push_back(subset[*set_iter]);
	}

	if(G.search_path_withset(start, end, gene, dist, v))
	{			
		return true; 
	}
	return false;	
}

/*
************************************************************************ 
*函数功能：   判断染色体是否合法   
*函数参数：   染色体表示的vector容器	
*函数返回值： 返回布尔型，若染色体合法返回true， 否则返回false 		        
************************************************************************
*/
bool GeneAlg::gene_judge(vector<int> &gene)
{
	if(gene.size() < 2)
	{
		return false;
	}
	else
	{
		for(vector<int>::iterator it=gene.begin(); it!=gene.end()-1; ++it)
		{
			if(!G.edge_exist(*it, *(it+1)))
			{
				return false; 
			}
		}
		return true;
	}
}

/*
************************************************************************ 
*函数功能：   计算染色体代表路径的长度   
*函数参数：   染色体表示的vector容器	
*函数返回值： 返回整形，路径的长度		        
************************************************************************
*/
int GeneAlg::get_gene_course(vector<int> &gene)
{
	int sum = 0;
	for(vector<int>::iterator it=gene.begin(); it!=gene.end()-1; ++it)
	{
		sum += G.get_edge_cost(*it, *(it+1));
	}
	return sum;
}

/*
************************************************************************ 
*函数功能：   获得染色体中经过子集节点的数目
*函数参数：   染色体表示的vector容器	
*函数返回值： 返回整形，经过子集节点的数目		        
************************************************************************
*/
int GeneAlg::get_gene_point(vector<int> &gene)
{
	int sum = 0;
	for(vector<int>::iterator it=subset.begin(); it!=subset.end(); ++it)
	{
		for(vector<int>::iterator itg=gene.begin(); itg!=gene.end(); ++itg)
		{
			if(*itg == *it)
			{
//				cout << *it << endl;
				sum++;
			}
		}
	}
	return sum;
}

/*
************************************************************************ 
*函数功能：   交叉算子，将x1染色体和x2染色体进行交叉
*函数参数：   x1，x2表示染色体编号,gene1和gene2存储交叉后生成的新染色体	
*函数返回值： 返回布尔型，交叉成功返回true，失败返回false		        
************************************************************************
*/
bool GeneAlg::xover(int x1, int x2)
{
	vector<int> s;
	if(!same_point(population[x1].gene, population[x2].gene, s))
	{
		return false;
	}
	s.erase(s.begin());
	s.erase(s.end()-1);

	while(!s.empty())
	{	
		int x=rand()%(s.size());
		vector<int>::iterator it1 = find(population[x1].gene.begin(),
						    population[x1].gene.end(), s[x]);
		vector<int>::iterator it2 = find(population[x2].gene.begin(),
							population[x2].gene.end(), s[x]);
		vector<int> result1(population[x1].gene.begin(), it1);
		vector<int> result2(population[x2].gene.begin(), it2);
		vector<int> end1(it1, population[x1].gene.end());
		vector<int> end2(it2, population[x2].gene.end());
		if(result1==result2 || end1==end2)
		{
			s.erase(s.begin()+x);
		}
		else
		{
			result1.insert(result1.end(), it2, population[x2].gene.end());
			result2.insert(result2.end(), it1, population[x1].gene.end());
			loop_remove(result1);
			loop_remove(result2);
			int point_raw1 = get_gene_point(population[x1].gene);
			int point_raw2 = get_gene_point(population[x2].gene);
			int point_new1 = get_gene_point(result1);
			int point_new2 = get_gene_point(result2);
			if(point_new1 >= point_raw1)
			{
				population[x1].gene = result1;
			}
			if(point_new2 >= point_raw2)
			{
				population[x2].gene = result2;
			}
			return true;
		}
	}
	return false;
}

/*
************************************************************************ 
*函数功能：   第二种交叉算子，将x1染色体和x2染色体进行交叉
*函数参数：   x1，x2表示染色体编号	
*函数返回值： 返回布尔型，交叉成功返回true，失败返回false		        
************************************************************************
*/
bool GeneAlg::xover2(int x1, int x2)
{

	int i = rand()%(population[x1].gene.size()-2) + 1;
	int j = rand()%(population[x2].gene.size()-2) + 1;
	if(i == j)
	{
		return false;
	}
	vector<int> s1;
	vector<int> s2;
	vector<int> result1;
	vector<int> result2;
	int temp;
	if(G.search_path(population[x1].gene[i],population[x2].gene[j],s1, temp))
	{
		return false;
	}
	if(G.search_path(population[x2].gene[j],population[x1].gene[i],s2, temp))
	{
		return false;
	}

	result1.insert(result1.end(),population[x1].gene.begin(),population[x1].gene.begin()+i);
	result1.insert(result1.end(),s1.begin(),s1.end());
	result1.insert(result1.end(),population[x2].gene.begin()+j,population[x2].gene.end());
	
	result2.insert(result2.end(),population[x2].gene.begin(),population[x2].gene.begin()+j);
	result2.insert(result2.end(),s2.begin(),s2.end());
	result2.insert(result2.end(),population[x1].gene.begin()+i,population[x1].gene.end());
	
	loop_remove(result1);
	loop_remove(result2);

	int point_raw1 = get_gene_point(population[x1].gene);
	int point_raw2 = get_gene_point(population[x2].gene);
	int point_new1 = get_gene_point(result1);
	int point_new2 = get_gene_point(result2);
	if(point_new1 >= point_raw1)
	{
		population[x1].gene = result1;
	}
	if(point_new2 >= point_raw2)
	{
		population[x2].gene = result2;
	}

	return true;	
}

/*
************************************************************************ 
*函数功能：   判断染色体中是否出现了回路
*函数参数：   染色体编码, elem存储产生回路的元素	
*函数返回值： 返回布尔型，存在回路返回true，否则返回false		        
************************************************************************
*/
bool GeneAlg::loop_judge(vector<int> &gene, int &elem)
{
	set<int> s;
	pair<set<int>::iterator,bool> flag;
	for(vector<int>::iterator it=gene.begin(); it!=gene.end(); ++it)
	{
		flag = s.insert(*it);
		if(flag.second == false)
		{
			elem = *it;
			return true;
		}
	}
	return false;
}

/*
************************************************************************ 
*函数功能：   移除染色体中的回路
*函数参数：   染色体编码	
*函数返回值： 无		        
************************************************************************
*/
void GeneAlg::loop_remove(vector<int> &gene)
{
	int elem;
	while(loop_judge(gene, elem))
	{
		vector<int>::iterator start, end;
		start = find(gene.begin(), gene.end(), elem);
		end = find(start+1, gene.end(), elem);
		gene.erase(start, end);
	}
}

/*
************************************************************************ 
*函数功能：   查找两条染色体的相同基因
*函数参数：   gene1， gene2为被查找的染色体,s存储相同的基因	
*函数返回值： 返回布尔型,若存在相同点，则返回true， 否则返回false		        
************************************************************************
*/
bool GeneAlg::same_point(vector<int> &gene1, vector<int> &gene2, vector<int> &s)
{
	s.clear();
	for (vector<int>::iterator it=gene1.begin(); it!=gene1.end(); ++it)
	{
		if(find(gene2.begin(),gene2.end(),*it) != gene2.end())
		{
			s.push_back(*it);
		}
	}
	if(s.size()==2)
	{
		return false;
	}
	return true;
}

/*
************************************************************************ 
*函数功能：   查找染色体中尚未包含的必经点
*函数参数：   gene为被查找的染色体,s结果节点	
*函数返回值： 无		        
************************************************************************
*/
void GeneAlg::remain_point(vector<int> &gene, vector<int> &s)
{
	s.clear();
	for (vector<int>::iterator it=subset.begin(); it!=subset.end(); ++it)
	{
		if(find(gene.begin(),gene.end(),*it) == gene.end())
		{
			s.push_back(*it);
		}
	}
}

/*
************************************************************************ 
*函数功能：   查找染色体中已经包含的必经点
*函数参数：   gene为被查找的染色体,s结果节点集合	
*函数返回值： 无		        
************************************************************************
*/
void GeneAlg::include_point(vector<int> &gene, vector<int> &s)
{
	s.clear();
	for (vector<int>::iterator it=subset.begin(); it!=subset.end(); ++it)
	{
		if(find(gene.begin(),gene.end(),*it) != gene.end())
		{
			s.push_back(*it);
		}
	}
}

/*
************************************************************************ 
*函数功能：   对染色体进行变异, p为变异点
*函数参数：   gene为变异的染色体
*函数返回值： 返回布尔型，变异成功返回true，否则返回false	        
************************************************************************
*/
bool GeneAlg::mutate_insert(vector<int> &gene, int p, vector<int> &s)
{
	vector<int> result;
	vector<int> temp;
	vector<int> path_temp;
	int dist;
	temp.push_back(s[p]);
	int pos = rand() % (gene.size()-1);
//	cout << "pos: " << pos << endl;
	if(G.search_path_withset(gene[pos], gene[pos+1], path_temp, dist, temp))
	{
		result.insert(result.end(), gene.begin(), gene.begin()+pos);
		result.insert(result.end(), path_temp.begin(), path_temp.end()-1);
		result.insert(result.end(), gene.begin()+pos+1, gene.end());
		loop_remove(result);
		if(get_gene_point(gene) <= get_gene_point(result)-1)
		{
			gene = result;
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool GeneAlg::mutate_random(vector<int> &gene)
{
	int temp;
	int a = rand()%(gene.size()-1) + 1;
	int b = rand()%(gene.size()-1) + 1;		
	vector<int> v;
	if(a == b)
	{
		return false;
	}
	if(a > b)
	{
		temp = a;
		a = b;
		b = temp;
	}
	if(G.search_path_random(gene[a], gene[b], v))
	{		
		gene.erase(gene.begin()+a, gene.begin()+b);
		gene.insert(gene.begin()+a, v.begin(), v.end());
		loop_remove(gene);
		return true;
	}
	return false;
}

bool GeneAlg::mutate(vector<int> &gene)
{
	vector<int> result;
	vector<int> s;
	int p;
	remain_point(gene, s);
	if(s.empty())
	{
		return false;
	}
	for(int i=0; i<3; i++)
	{
		p = rand() % (s.size());
		if(mutate_insert(gene, p, s))
		{
			return true;
		}
	}
	return false;
}

/*
************************************************************************ 
*函数功能：   寻找种群中经过所有必经点的最优个体
*函数参数：   无	
*函数返回值： 无		        
************************************************************************
*/
void GeneAlg::keep_the_point_best()
{
	for(int i=0; i<POP_SIZE; i++)
	{
		if(population[i].point_num == subset.size())
		{
			if(population[i].course < better_point.course)
			{
				better_point = population[i];
			}
		}
	}
}

/*
************************************************************************ 
*函数功能：   计算种群中所有个体的参数
*函数参数：   无	
*函数返回值： 无		        
************************************************************************
*/
void GeneAlg::evaluate()
{
	double sum_path_fit = 0.0;
	double sum_point_fit = 0.0;
	for (int i=0; i<POP_SIZE; i++)
	{
		population[i].course = get_gene_course(population[i].gene);
		population[i].point_num = get_gene_point(population[i].gene);
		population[i].path_fit = 1/(double)population[i].course;
		population[i].point_fit = population[i].point_num/(double)(subset.size());
		sum_point_fit += population[i].point_fit;
		sum_path_fit += population[i].path_fit;
	}
	sum_fit = 0.0;
	for(int i=0; i<POP_SIZE; i++)
	{
		population[i].path_rfit = population[i].path_fit / sum_path_fit;
		population[i].point_rfit = population[i].point_fit / sum_point_fit;
		population[i].fit = POINT_PER * population[i].point_rfit 
									+ COURSE_PER * population[i].path_rfit;
		sum_fit += population[i].fit;
	}

	for(int i=0; i<POP_SIZE; i++)
	{
		population[i].rfit = population[i].fit / sum_fit;
	}

	population[0].cfit = population[0].rfit;
	for(int i=1; i<POP_SIZE; i++)
	{
		population[i].cfit = population[i-1].cfit + population[i].rfit;
	}
}




