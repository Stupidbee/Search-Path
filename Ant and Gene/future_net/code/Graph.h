/*
*********************************************************************************************************
* File      : Graph.h
* Version   : V1.0
* By        : Zuxiang Wang
* Function  : 声明了图类，定义了相关内联函数 
* Toolchain : g++ or vs2015
*********************************************************************************************************
*/
#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <stack>
#include <set>
#include <algorithm>
#include <fstream>


class GeneAlg;


using std::cout;
using std::endl;
using std::vector;
using std::stack;
using std::set;
using std::reverse;
using std::ostream;

//图的类声明
class Graph
{
public:
	Graph();
	~Graph();
	const Graph & operator = (const Graph &rhs);
	Graph(const Graph &rhs);
		
	friend std::ostream & operator <<(std::ostream &os, const Graph &rhs);
	friend class GeneAlg;

	inline int get_edge_cost(int v1, int v2) const;        //获得边的权值
	inline int get_edge_id(int v1, int v2) const;          //获得变的id编号
	inline int get_vertex_idegree(int v0) const;           //获得顶点的入度
	inline int get_vertex_odegree(int v0) const;           //获得顶点的出度

	inline int edges() const;                              //获得边属
	inline int vertexs() const;                            //获得顶点数

	inline bool vertex_exist(int v) const;                 //判断顶点的存在性
	inline bool edge_exist(int v1, int v2) const;          //判断边的存在性
	
	inline bool vertex_add(int v);                         //添加一个顶点                   
	inline bool edge_add(int v1, int v2, int cost, int id);//添加一条边

	inline bool vertex_del(int v1);                        //删除一个顶点
	
	bool edge_del(int v1, int v2);                         //删除一条边	

	inline int get_vertex_index(int v);

	inline bool modify(int start_pt, int end_pt, set<int> &subset);
	bool search_path(int v1, int v2, vector<int> &result, int &dist);
	bool search_path_random(int v1, int v2, vector<int> &result);
	bool search_path_withset(int start, int end, vector<int> &result, int &dist, vector<int> subset);
	bool search_path_random_set(int v1, int v2, vector<int> &result, vector<int> subset);

private:
	//定义边的结构
	struct Edge
	{
		int cost;   //权值
		int id;     //边的id编号
	};

	int *vertex;    //顶点表
	Edge **edge;    //边的矩阵
	int vertex_num;
	int edge_num;

	inline int get_vertex_idegree_index(int index) const;
	inline int get_vertex_odegree_index(int index) const;

	inline bool trim(int start_pt, int end_pt, set<int> &subset);
	inline bool simple_graph(int start_pt, int end_pt, set<int> &subset) const;

	inline bool find_vertex_index(int v, int &index) const;  //寻找顶点在顶点表的位置索引
	void shortest_path_dijkstra(int v0, int *path, int *dist);

	static const int INF = 65535;       //没有边时的值，边的权值小于20
	static const int MAX = 600;      //最大顶点数
};

/*
************************************************************************ 
*函数功能：   获取边的数目
*函数参数：   无     		
*函数返回值： 返回整形，返回值为边的数目 		       
************************************************************************
*/
int Graph::edges() const
{
	return edge_num;
}

/*
************************************************************************ 
*函数功能：   获取顶点的数目
*函数参数：   无     		
*函数返回值： 返回整形，返回值为顶点的数目 		
        
************************************************************************
*/
int Graph::vertexs() const
{
	return vertex_num;
}

/*
************************************************************************ 
*函数功能：   判断顶点存在性 
*函数参数：   顶点的值     		
*函数返回值： 返回布尔型，顶点存在返回true，不存在返回false 		     
************************************************************************
*/
bool Graph::vertex_exist(int v) const
{
	for(int i=0; i<vertex_num; i++)
	{
		if(vertex[i] == v)
		{
			return true;
		}
	}
	return false; 
}

/*
************************************************************************ 
*函数功能：   判断边的存在性
*函数参数：   v1是边的入顶点，v2是边的出顶点     		
*函数返回值： 返回布尔型，边存在返回true，不存在返回false 		        
************************************************************************
*/
bool Graph::edge_exist(int v1, int v2) const
{
	int index_v1;
	int index_v2;
	if((!find_vertex_index(v1, index_v1)) || (!find_vertex_index(v2,index_v2)))
	{
//		cout << "Vertex is not exist" << endl;
		return false;
	}

	if(edge[index_v1][index_v2].cost < INF)
	{
		return true;
	}
	else
		return false;
}

/*
************************************************************************ 
*函数功能：   添加一个顶点
*函数参数：   顶点值     		
*函数返回值： 返回布尔型，添加顶点成功返回true，失败返回false 		        
************************************************************************
*/
bool Graph::vertex_add(int v)
{
	if(!vertex_exist(v))
	{
		vertex[vertex_num++] = v;
		return true;
	}
	return false;
}

/*
************************************************************************ 
*函数功能：   添加一条边
*函数参数：   v1是入顶点值，v2是出顶点值，cost是权值，id是边的id编号    		
*函数返回值： 返回布尔型，添加边成功返回true，失败返回false 		        
************************************************************************
*/
bool Graph::edge_add(int v1, int v2, int cost, int id)
{
	int index_v1;
	int index_v2;
	if((!find_vertex_index(v1, index_v1)) || (!find_vertex_index(v2,index_v2)))
	{
//		cout << "Vertex is not exist" << endl;
		return false;
	}

	if(edge[index_v1][index_v2].cost == INF)
	{
		edge[index_v1][index_v2].cost = cost;
		edge[index_v1][index_v2].id = id;
		edge_num++;
		return true;
	}

	else if(edge[index_v1][index_v2].cost > cost)
	{
		edge[index_v1][index_v2].cost = cost;
		edge[index_v1][index_v2].id = id;
		return true;
	}

	else
		return false;
}

/*
************************************************************************ 
*函数功能：   查询顶点在顶点表中的位置索引，该函数为private成员函数
*函数参数：   v是顶点值， index引用传递，是查询成功的位置索引     		
*函数返回值： 返回布尔型，查询顶点成功返回true，失败返回false 		        
************************************************************************
*/
bool Graph::find_vertex_index(int v, int &index) const
{
	for(int i=0; i<vertex_num; i++)
	{
		if(vertex[i] == v)
		{
			index = i;
			return true;
		}
	}
	return false; 
}

 int Graph::get_vertex_index(int v)
 {
 	for(int i=0; i<vertex_num; i++)
	{
		if(vertex[i] == v)
		{
			return i;
		}
	} 
 }

int Graph::get_edge_cost(int v1, int v2) const
{
	int index_v1;
	int index_v2;
	if((!find_vertex_index(v1, index_v1)) || (!find_vertex_index(v2,index_v2)))
	{
//		cout << "Vertex is not exist" << endl;
		return 0;
	}
	return edge[index_v1][index_v2].cost;
}

/*
************************************************************************ 
*函数功能：   查询边的id编号
*函数参数：   v1是边的入顶点值，v2是边的出顶点值     		
*函数返回值： 返回整形，返回值为边的id编号 		        
************************************************************************
*/
int Graph::get_edge_id(int v1, int v2) const
{
	int index_v1;
	int index_v2;
	if((!find_vertex_index(v1, index_v1)) || (!find_vertex_index(v2,index_v2)))
	{
//		cout << "Vertex is not exist" << endl;
		return false;
	}
	return edge[index_v1][index_v2].id;
}

/*
************************************************************************ 
*函数功能：   查询顶点的入度
*函数参数：   v0是待查询的顶点值		
*函数返回值： 返回整形，返回值为顶点的入度 		        
************************************************************************
*/
int Graph::get_vertex_idegree(int v0) const
{
	int index_v0;
	int num = 0;
	find_vertex_index(v0, index_v0);
	for(int i=0; i<vertex_num; i++)
	{
		//入度即为邻接矩阵中相应列不为0和INF的元素的个数
		if(edge[i][index_v0].cost!=INF && edge[i][index_v0].cost!=0)
		{
			num++;         
		}
	}
	return num;
}

/*
************************************************************************ 
*函数功能：   查询顶点的入度
*函数参数：   v0是待查询的顶点索引		
*函数返回值： 返回整形，返回值为顶点的入度 		        
************************************************************************
*/
int Graph::get_vertex_idegree_index(int index) const
{
	int num = 0;
	for(int i=0; i<vertex_num; i++)
	{
		//入度即为邻接矩阵中相应列不为0和INF的元素的个数
		if(edge[i][index].cost!=INF && edge[i][index].cost!=0)
		{
			num++;         
		}
	}
	return num;
}

/*
************************************************************************ 
*函数功能：   查询顶点的出度
*函数参数：   v0是待查询的顶点值		
*函数返回值： 返回整形，返回值为顶点的出度 		        
************************************************************************
*/
int Graph::get_vertex_odegree(int v0) const
{
	int index_v0;
	int num = 0;
	find_vertex_index(v0, index_v0);
	//出度即为邻接矩阵中相应行不为0和INF的元素的个数
	for(int i=0; i<vertex_num; i++)
	{
		if(edge[index_v0][i].cost!=INF && edge[index_v0][i].cost!=0)
		{
			num++;
		}
	}
	return num;
}

/*
************************************************************************ 
*函数功能：   查询顶点的出度
*函数参数：   v0是待查询的顶点索引		
*函数返回值： 返回整形，返回值为顶点的出度 		        
************************************************************************
*/
int Graph::get_vertex_odegree_index(int index) const
{
	int num = 0;
	//出度即为邻接矩阵中相应行不为0和INF的元素的个数
	for(int i=0; i<vertex_num; i++)
	{
		if(edge[index][i].cost!=INF && edge[index][i].cost!=0)
		{
			num++;
		}
	}
	return num;
}

/*
************************************************************************ 
*函数功能：   删除一个顶点
*函数参数：   v0是待删除的顶点值		
*函数返回值： 返回布尔型，删除成功返回true，失败返回false 		        
************************************************************************
*/
bool Graph::vertex_del(int v0)
{
	int index_v0;
	int tmp;
	Edge etmp;
	//获取该顶点的度，包含出度和入度
	int degree = get_vertex_odegree(v0) + get_vertex_idegree(v0); 

	if (find_vertex_index(v0, index_v0))
	{
		//在顶点表中删除顶点值，
		//将顶点表中相应位置的值置位-1，然后与最后一个顶点交换
		//保证顶点表的完整性
		vertex[index_v0] = -1;
		tmp = vertex[vertex_num-1];
		vertex[vertex_num-1] = vertex[index_v0];
		vertex[index_v0] = tmp;

		//删除该顶点的全部的入边
		for(int i=0; i<vertex_num; i++)
		{
			etmp = edge[i][vertex_num-1];
			edge[i][vertex_num-1] = edge[i][index_v0];
			edge[i][index_v0] = etmp;
		}	
		
		//删除该顶点的全部的出边
		for(int i=0; i<vertex_num; i++)
		{
			etmp = edge[vertex_num-1][i];
			edge[vertex_num-1][i] = edge[index_v0][i];
			edge[index_v0][i] = etmp;
		}

		for(int i=0; i<vertex_num; i++)
		{
			edge[vertex_num-1][i].cost = INF;
			edge[vertex_num-1][i].id = -1;
			edge[i][vertex_num-1].cost= INF;
			edge[i][vertex_num-1].id = -1;			
		}

		edge_num -= degree;    //更新边数
		vertex_num--;          //更新顶点数
		return true;
	}
	return false;
}

/*
************************************************************************ 
*函数功能：   对图进行一次剪枝
*函数参数：   start_pt是起点值，end_pt是终点值，subset是需要经过的点集		
*函数返回值： 返回布尔型，成功返回true，失败返回false，失败表示无路径		        
************************************************************************
*/
bool Graph::trim(int start_pt, int end_pt, set<int> &subset)
{
	set<int>::iterator it;
	for(int i=0; i<vertex_num; i++)
	{
		if(get_vertex_odegree_index(i)==0 && vertex[i] != end_pt)
		{
			it = subset.find(vertex[i]);
			if(it != subset.end() || vertex[i] == start_pt)
			{
				return false;
			}
			else
			{
				vertex_del(vertex[i]);				
			}
			
		}
	}

	for(int i=0; i<vertex_num; i++)
	{
		if(get_vertex_idegree_index(i)==0 && vertex[i] != start_pt)
		{
			it = subset.find(vertex[i]);			
			if(it != subset.end() || vertex[i] == end_pt)
			{
				return false;
			}
			else
			{				
				vertex_del(vertex[i]);
			}
			
		}
	}
	return true;
}

/*
************************************************************************ 
*函数功能：   判断图需不需要剪枝
*函数参数：   start_pt是起点值，end_pt是终点值，subset是需要经过的点集		
*函数返回值： 返回布尔型，是最简图返回true，否则返回false		        
************************************************************************
*/
bool Graph::simple_graph(int start_pt, int end_pt, set<int> &subset) const
{
	set<int>::iterator it;
	for(int i=0; i<vertex_num; i++)
	{
		it = subset.find(vertex[i]);
		if(
			(get_vertex_odegree_index(i)==0 ||  
		    get_vertex_idegree_index(i)==0) &&
		    vertex[i] != end_pt             &&
		    vertex[i] != start_pt           &&
		    it == subset.end()
		) 
		{
			return false;
		}
	}
	return true;	
}

/*
************************************************************************ 
*函数功能：   对图进行剪枝
*函数参数：   start_pt是起点值，end_pt是终点值，subset是需要经过的点集		
*函数返回值： 返回布尔值，返回true表示剪枝成功，返回false表示没有路径		        
************************************************************************
*/
bool Graph::modify(int start_pt, int end_pt, set<int> &subset)
{
	while(!simple_graph(start_pt, end_pt, subset))
	{
		if(trim(start_pt, end_pt, subset))
		{
			continue;
		}
		else
		{
//			cout << "No path in subset" <<endl;
			return false;
		}
	}
	return true;
}

#endif