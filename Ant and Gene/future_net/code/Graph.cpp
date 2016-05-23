/*
*********************************************************************************************************
* File      : Graph.h
* Version   : V1.0
* By        : Zuxiang Wang
* Function  : 定义实现了图的成员函数 
* Toolchain : g++ or vs2015
*********************************************************************************************************
*/

#include "Graph.h"

void print(vector<int> &v, ostream &os);
/*
************************************************************************ 
*函数功能：   构造函数，建立图对象，初始化成员变量
*函数参数：   无     		
*函数返回值： 图对象		       
************************************************************************
*/

Graph::Graph()
{
	//开始没有顶点
	vertex = new int[MAX];
	for (int i=0; i<MAX; i++)
	{
		vertex[i] = -1; 
	}

	//开始没有边，全部初始化为INF
	edge = new Edge *[MAX];
	for (int i=0; i<MAX; i++)
	{
		edge[i] = new Edge[MAX];
	}
	for (int i=0; i<MAX; i++)
	{
		for (int j=0; j<MAX; j++)
		{
			if(i == j)
			{
				edge[i][j].id = -1;
				edge[i][j].cost = 0;
			}
			else
			{
				edge[i][j].id = -1;
				edge[i][j].cost = INF;
			}
			
		}
	}
	vertex_num = 0;
	edge_num = 0;
}

/*
************************************************************************ 
*函数功能：   拷贝构造函数，建立图对象，初始化成员变量
*函数参数：   图对象的引用    		
*函数返回值： 图对象		       
************************************************************************
*/
Graph::Graph(const Graph &rhs)
{
	vertex = new int[MAX];
	for (int i=0; i<MAX; i++)
	{
		vertex[i] = rhs.vertex[i]; 
	}
	edge = new Edge *[MAX];
	for (int i=0; i<MAX; i++)
	{
		edge[i] = new Edge[MAX];
	}
	for (int i=0; i<MAX; i++)
	{
		for (int j=0; j<MAX; j++)
		{
			edge[i][j] = rhs.edge[i][j];		
		}
	}
	vertex_num = rhs.vertex_num;
	edge_num = rhs.edge_num;
}

/*
************************************************************************ 
*函数功能：   重载赋值运算符，建立图对象，初始化成员变量
*函数参数：   图对象的引用    		
*函数返回值： 图对象		       
************************************************************************
*/
const Graph & Graph::operator = (const Graph &rhs)
{
	if(this == &rhs)
	{
		return *this;
	}

	for(int i=0;i<MAX;i++)
	{
		delete [] edge[i];
	}
	delete [] edge;
	delete [] vertex;

	vertex = new int[MAX];
	for (int i=0; i<MAX; i++)
	{
		vertex[i] = rhs.vertex[i]; 
	}
	edge = new Edge *[MAX];
	for (int i=0; i<MAX; i++)
	{
		edge[i] = new Edge[MAX];
	}
	for (int i=0; i<MAX; i++)
	{
		for (int j=0; j<MAX; j++)
		{
			edge[i][j] = rhs.edge[i][j];			
		}
	}
	vertex_num = rhs.vertex_num;
	edge_num = rhs.edge_num;
	return *this;
}

/*
************************************************************************ 
*函数功能：   析构函数，释放对象空间
*函数参数：   无     		
*函数返回值： 无		       
************************************************************************
*/
Graph::~Graph()
{
	for(int i=0;i<MAX;i++)
	{
		delete [] edge[i];
	}
	delete [] edge;
	delete [] vertex;
}

/*
************************************************************************ 
*函数功能：   删除一条边
*函数参数：   v1是待删除边的入顶点，v2是待删除边的出顶点		
*函数返回值： 返回布尔型，删除成功返回true，失败返回false 		        
************************************************************************
*/
bool Graph::edge_del(int v1, int v2)
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
		edge[index_v1][index_v2].cost = INF;
		edge[index_v1][index_v2].id = -1;
		edge_num--;
		return true;
	}
	else
		return false;
}

/*
************************************************************************ 
*函数功能：   重载了<<操作符
*函数参数：   os是输出流对象的引用，rhs是图的引用		
*函数返回值： 返回输出流对象的引用 		        
************************************************************************
*/
std::ostream & operator <<(std::ostream &os, const Graph &rhs)
{
	os << "Vertex is: ";
	for(int i=0; i<rhs.vertex_num; i++)
	{
		os << rhs.vertex[i] << " ";
	}
	os << std::endl;

	os << "Edge is:" << std::endl;
	for(int i=0; i<rhs.vertex_num; i++)
	{
		for(int j=0; j<rhs.vertex_num; j++)
		{
			os << std::setw(5) << rhs.edge[i][j].cost << " ";
		}
		os << std::endl;
	}
	os << std::endl;
	return os;
}

/*
************************************************************************ 
*函数功能：   迪杰斯特拉算法的实现函数，该函数为private成员
*函数参数：   v0是起始顶点的索引，path存储路径表，dist存储到各个点的最短路径值	
*函数返回值： 无		        
************************************************************************
*/
void Graph::shortest_path_dijkstra(int v0, int *path, int *dist)
{
	int min;
	int k = 0;
	int *visited = new int[vertex_num];

	//初始化,起始状态下各个点与v0的距离
	for(int i=0; i<vertex_num; i++)
	{
		visited[i] = false;
		dist[i] = edge[v0][i].cost;
		path[i] = v0;
	} 
	visited[v0] = true;
	dist[v0] = 0;

	for(int i=1; i<vertex_num; i++)
	{
		min = INF;
		for(int j=0; j<vertex_num; j++)
		{
			if(!visited[j] && dist[j]<min)
			{
				min = dist[j];
				k = j;
			}
		}
		visited[k] = true;
		for(int j=0; j<vertex_num; j++)
		{
			if(!visited[j] && (min+edge[k][j].cost < dist[j]))
			{
				dist[j] = min + edge[k][j].cost;
				path[j] = k;
			}
		}
	}
	delete [] visited;
}

/*
************************************************************************ 
*函数功能：   调用迪杰斯特拉算法，寻找一点到另一点的最短路径和路径权值
*函数参数：   v1是起点值，v2是终点值，result存储找到的路径，dst存储路径权值	
*函数返回值： 返回布尔型，若v1到v2有路径返回true，无路径返回false		        
************************************************************************
*/
bool Graph::search_path(int v1, int v2, vector<int> &result, int &dst)
{
	int index_v1, index_v2;
	std::stack<int> s;
	int tmp;
	int *path = new int[vertex_num];
	int *dist = new int[vertex_num];

	if((!find_vertex_index(v1, index_v1))|| (!find_vertex_index(v2, index_v2)))
	{
//		cout << "No vertex" << endl;
		return false;
	}
	shortest_path_dijkstra(index_v1, path, dist);
	result.clear();
//	s.push(index_v2);
	tmp = path[index_v2];
	while(tmp != index_v1)
	{
		s.push(tmp);
		tmp = path[tmp];
	}	

//	s.push(index_v1);
	while(!s.empty())
	{
		tmp = s.top();
		s.pop();
		result.push_back(vertex[tmp]);
	}

	dst = dist[index_v2];
	if(result.empty() && dst==INF)
	{
//		result.clear();
		return false;
	}	
	delete [] path;
	delete [] dist;
	return true;
}

bool Graph::search_path_withset(int start, int end, vector<int> &result, int &dist, vector<int> subset)
{
	vector<int> v;
	dist = 0;
	int dist_temp;

	subset.insert(subset.begin(), start);
	subset.push_back(end);
	vector<int>::iterator it;
	Graph G(*this);
	while(subset.size()!=1)
	{		
		it = subset.begin();
		if(G.search_path(*it, *(it+1), v, dist_temp))
		{
			dist += dist_temp;
			result.push_back(*it);
			G.vertex_del(*it);
			subset.erase(subset.begin());	
			for(vector<int>::iterator itv=v.begin();itv!=v.end(); ++itv)
			{
				if(*itv == end)
				{
					result.clear();
					return false;
				}
				result.push_back(*itv);
				G.vertex_del(*itv);
			}
			for(vector<int>::iterator itv=v.begin();itv!=v.end(); ++itv)
			{	
				for(vector<int>::iterator its=subset.begin(); its!=subset.end()-1;)    
				{
        			if(*its == *itv)
        			{
             			its = subset.erase(its);    
        			}
         			else
         			{
            			++its; 
         			}
     			}
			}
		}
		else
		{
			result.clear();
			dist = 0;
			return false;
		}
	}
	result.push_back(end);
	return true;	
}

bool Graph::search_path_random(int v1, int v2, vector<int> &result)
{
	int v1_index;
	int v2_index;
	if((!find_vertex_index(v1, v1_index))|| (!find_vertex_index(v2, v2_index)))
	{
//		cout << "No vertex" << endl;
		return false;
	}
	result.clear();
	bool *visited = new bool[vertex_num];
	stack<int> s;
	for(int i=0; i<vertex_num; i++)
	{
		visited[i] = false;
	}
	vector<int> v;
	s.push(v1_index);
	visited[v1_index] = true;
	while(!s.empty())
	{
		if(s.top()!=v2_index)
		{
			for(int i=0; i<vertex_num; i++)
			{
				if(!visited[i]&&edge[s.top()][i].cost!=INF&&edge[s.top()][i].cost!=0)
				{
					v.push_back(i);
				}
			}

			if(!v.empty())
			{
				int k = v[rand()%(v.size())];
				s.push(k);
				visited[k] = true;
				v.clear();
			}
			else
			{
				s.pop();
			}
		}
		else
		{
			while(!s.empty())
			{
				result.push_back(vertex[s.top()]);
				s.pop();
			}
			reverse(result.begin(), result.end());
			delete [] visited;
			return true;
		}
	} 
	delete [] visited;
	return false;
}

bool Graph::search_path_random_set(int v1, int v2, vector<int> &result, vector<int> subset)
{
	Graph G(*this);
	vector<int> temp;
	int a;
	result.push_back(v1);
	subset.push_back(v2);
	vector<int>::iterator it;
	while(subset.size()!=1)
	{
		it = result.end() - 1;
		a = rand()%(subset.size()-1);
		if(G.search_path_random(*it, subset[a], temp))
		{
			result.insert(result.end(), temp.begin()+1, temp.end());
			for(vector<int>::iterator itv=temp.begin();itv!=temp.end()-1; ++itv)
			{

				if(*itv == v2)
				{
//					cout << "fail end" << endl;
					result.clear();
					return false;
				}
				G.vertex_del(*itv);
			}
			for(vector<int>::iterator itv=temp.begin();itv!=temp.end(); ++itv)
			{	
				for(vector<int>::iterator its=subset.begin(); its!=subset.end()-1;)    
				{
        			if(*its == *itv)
        			{
             			its = subset.erase(its);    
        			}
         			else
         			{
            			++its; 
         			}
     			}
			}
		}
		else
		{
			result.clear();
			return false;
		}
	}
	result.push_back(v2);
	return true;
}








