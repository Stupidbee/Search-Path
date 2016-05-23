#include "route.h"
#include "lib_record.h"
#include <stdio.h>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <ctime>

using namespace std;
//你要完成的功能总入口

int cal_edge(vector<int> &r1, vector<int> &r2);
int Total_num(int Point_num[], int edge_num);

void search_route(char *topo[MAX_EDGE_NUM], int edge_num, char *demand[MAX_DEMAND_NUM], int demand_num)
{
	cout << "start time:" << clock()*1000/CLOCKS_PER_SEC << "ms"<< endl;
//    unsigned short result1[] = {0, 1, 2};//P'路径
//    unsigned short result2[] = {5, 6, 2};//P''路径

    vector<int> result_t1, point1;
    vector<int> result_t2, point2;

    init();
    create_graph(topo, edge_num, demand, demand_num);

#if 1
    search_route_ANT(demand_node1, result_t1, point1);
 /*   cout << "path 1" << endl;
    for (int i = 0; i < point1.size(); i++)
    {
        cout << point1[i] << " ";
    }
    cout << endl;
*/
#endif

#if 1
    flag = 2;
    for(int i=0;i<point1.size();i++)
    {
       if(max_ver<200)
	   g_cost[point1[i]][point1[i+1]]=55;
	   if(max_ver<501)
       g_cost[point1[i]][point1[i+1]]=200;
	   if(max_ver<1000)
       g_cost[point1[i]][point1[i+1]]=350;
	   if(max_ver<1500)
       g_cost[point1[i]][point1[i+1]]=500;
	   if(max_ver<2005)
       g_cost[point1[i]][point1[i+1]]=500;
    }
    search_route_ANT(demand_node2, result_t2, point2);
/*
    cout << "path 2" << endl;
    for (int i = 0; i < point2.size(); i++)
    {
    	cout << point2[i] << " ";
    }
    cout << endl;
*/
#endif

    for (int i = 0; i < result_t1.size(); i++)
    {
        record_result(WORK_PATH, result_t1[i]);
    }
    for (int i = 0; i < result_t2.size(); i++)
    {
    	record_result(BACK_PATH, result_t2[i]);
    }

    cout << "edge_num: " << cal_edge(result_t1, result_t2) << endl; 
    cout << "end time: " << clock()*1000/CLOCKS_PER_SEC << "ms"<< endl; 
}

void create_graph(char *topo[MAX_EDGE_NUM], int edge_num, char *demand[MAX_DEMAND_NUM], int demand_num)
{
        stringstream stream;
	stringstream stream1;
        stringstream stream2;

	int start;
	int end;
	int temp;
	int n;
	
	int a,b,c,d;
	char ch;

    int Point_Num[edge_num];
	int P_Num = 0;
    
	edge_num_ant = edge_num;
    for(int i = 0;i<edge_num;i++)
	{
		Point_Num[i] = 0; 
	}
	
	for(int i=0; i<edge_num; i++)
	{
		stream << topo[i];
		stream >> a >> ch >> b >> ch >> c >> ch >> d;
     	Point_Num[b]=b;
		Point_Num[c]=c;

		out_way[b].push_back(c);
		in_way[c].push_back(b);

		if(g_cost[b][c] > d) 
		{
			g_cost[b][c] = d;
			id[b][c] = a; 
		}

		stream.clear();
	}
	
	bool na_flag = true;
	stream1 << demand[0];
	stream1 >> n >> ch >> start >> ch >> end >> ch;
	start_point = start;
	end_point = end;

	string sd1(demand[0]);
	if(sd1.find('N')!=string::npos)
	{
		na_flag = false;
	}

	if(na_flag)
	{
		while(stream1 >> temp)
		{
			stream1 >> ch;
			demand_node1.insert(temp);
		}
	}

        na_flag = true;

	string sd2(demand[1]);
	if(sd2.find('N')!=string::npos)
	{
		na_flag = false;
	}

	stream2 << demand[1];
	stream2 >> n >> ch >> start >> ch >> end >> ch;
	if(na_flag)
	{
		while(stream2 >> temp)
		{
			stream2 >> ch;	
			demand_node2.insert(temp);
		}
	}

     for(int i=0;i<edge_num;i++)
	{
		if(Point_Num[i] != 0)
		P_Num++;
	}
	 max_ver = P_Num;
//	 cout<<"Total Num： " << max_ver<<endl;


}

bool search_route_ANT(set<int> &demand, vector<int> &result, vector<int> &point)
{
	if(max_ver>1500)
	{	
	ant_num = 180;
	iter_num = 70;
	}	
	else
	{
	ant_num = max_ver/5+50;
	iter_num = 200; //edge_num_ant / 30 + 1;
	}   
  // cout <<" max_ver: "<< max_ver<<endl;
  // cout << "num_ant:"<<ant_num<<endl;
  // cout << "iter_num "<<iter_num<<endl;
	DPSP dpsp(demand);
	dpsp.init_data();
	dpsp.search();
	if(dpsp.report(result, point))
	{
		return true;
	}
	else
	{
		return false;
	}
}

int cal_edge(vector<int> &r1, vector<int> &r2)
{
	int count = 0;
	for(auto it=r1.begin(); it!=r1.end(); ++it)
	{
		if(find(r2.begin(), r2.end(), *it) != r2.end())
		{
			count ++;
		}
	}
	return count;
}

