#include "DPSP_MIP.h"
#include <ctime>

using namespace std;

int result_num = 0;
int edge_index[MAX][MAX];
EdgeType edge_set[MAX_EDGE];
unordered_set<int> out_way[MAX];
unordered_set<int> in_way[MAX];
unordered_set<int> demand_node;
unordered_set<int> node;
glp_prob *P;
int out_way_path[600];

int ind[MAX_EDGE];
double val[MAX_EDGE];

void build_prob(void)
{
	int k = 0;
	int kk = 0;
	P = glp_create_prob();	
	glp_term_out(GLP_OFF);
	glp_add_cols(P, edge_num);

	//设置边的约束，及目标函数
	for(int i=1; i<=edge_num; i++)
	{
		k++;
		int start = edge_set[k].start;
		int end = edge_set[k].end;
		glp_set_col_kind(P, k, GLP_BV);
		glp_set_obj_coef(P, k, g_cost[start][end]);
	}

	//起点约束,出度
	k = glp_add_rows(P, 1);
	glp_set_row_bnds(P, k, GLP_FX, 1, 1);
	kk = 0;
	for(auto elem : out_way[start_point])
	{
		kk++;
		ind[kk] = edge_index[start_point][elem];
		val[kk] = 1;
	}
	
	glp_set_mat_row(P, k, kk, ind, val);
	////起点约束,入度
	k = glp_add_rows(P, 1);
	glp_set_row_bnds(P, k, GLP_FX, 0, 0);
	kk = 0;
	for(auto elem : in_way[start_point])
	{
		kk++;
		ind[kk] = edge_index[elem][start_point];
		val[kk] = 1;
	}	
	glp_set_mat_row(P, k, kk, ind, val);

	//终点约束，出度
	k = glp_add_rows(P, 1);
	glp_set_row_bnds(P, k, GLP_FX, 0, 0);
	kk = 0;
	for(auto elem : out_way[end_point])
	{
		kk++;
		ind[kk] = edge_index[end_point][elem];
		val[kk] = 1;
	}	
	glp_set_mat_row(P, k, kk, ind, val);

	//终点约束，入度
	k = glp_add_rows(P, 1);
	glp_set_row_bnds(P, k, GLP_FX, 1, 1);
	kk = 0;
	for(auto elem : in_way[end_point])
	{
		kk++;
		ind[kk] = edge_index[elem][end_point];
		val[kk] = 1;
	}	
	glp_set_mat_row(P, k, kk, ind, val);

	//必经点约束，出度
	for(auto it=demand_node.begin(); it!=demand_node.end(); ++it)
	{		
		k = glp_add_rows(P, 1);
		glp_set_row_bnds(P, k, GLP_FX, 1, 1);
		kk = 0;
		for(auto ito=out_way[*it].begin(); ito!=out_way[*it].end(); ++ito)
		{
			kk++;
			ind[kk] = edge_index[*it][*ito];
			val[kk] = 1;
		}
		glp_set_mat_row(P, k, kk, ind, val);
	}

	//必经点约束，入度
	for(auto it=demand_node.begin(); it!=demand_node.end(); ++it)
	{		
		k = glp_add_rows(P, 1);
		glp_set_row_bnds(P, k, GLP_FX, 1, 1);
		kk = 0;
		for(auto iti=in_way[*it].begin(); iti!=in_way[*it].end(); ++iti)
		{
			kk++;
			ind[kk] = edge_index[*iti][*it];
			val[kk] = 1;
		}
		glp_set_mat_row(P, k, kk, ind, val);
	}

	//其他点约束，出度
	for(auto it=node.begin(); it!=node.end(); ++it)
	{
		if(*it!=end_point && *it!=start_point && demand_node.count(*it)==0)
		{
			k = glp_add_rows(P, 1);
			glp_set_row_bnds(P, k, GLP_DB, 0, 1);
			kk = 0;
			for(auto ito=out_way[*it].begin(); ito!=out_way[*it].end(); ++ito)
			{
				kk++;
				ind[kk] = edge_index[*it][*ito];
				val[kk] = 1;
			}
			glp_set_mat_row(P, k, kk, ind, val);
		}		

	}
	
	//其他点约束，入度
	for(auto it=node.begin(); it!=node.end(); ++it)
	{
		if(*it!=end_point && *it!=start_point && demand_node.count(*it)==0)	
		{
			k = glp_add_rows(P, 1);
			glp_set_row_bnds(P, k, GLP_DB, 0, 1);
			kk = 0;
			for(auto iti=in_way[*it].begin(); iti!=in_way[*it].end(); ++iti)
			{
				kk++;
				ind[kk] = edge_index[*iti][*it];
				val[kk] = 1;
			}
			glp_set_mat_row(P, k, kk, ind, val);
		}
	}

	//其他点约束，出入度相等
	for(auto it=node.begin(); it!=node.end(); ++it)
	{
		if(*it!=end_point && *it!=start_point && demand_node.count(*it)==0)	
		{	
			k = glp_add_rows(P, 1);
			glp_set_row_bnds(P, k, GLP_FX, 0, 0);
			kk = 0;
			for(auto ito=out_way[*it].begin(); ito!=out_way[*it].end(); ++ito)
			{
				kk++;
				ind[kk] = edge_index[*it][*ito];
				val[kk] = 1;
			}
			for(auto iti=in_way[*it].begin(); iti!=in_way[*it].end(); ++iti)
			{
				kk++;
				ind[kk] = edge_index[*iti][*it];
				val[kk] = -1;
			}
			glp_set_mat_row(P, k, kk, ind, val);
		}
	}

}

void get_solve(vector<int> &result)
{
	result.clear();

	glp_iocp iocp;
	glp_init_iocp(&iocp);

	iocp.msg_lev = GLP_MSG_OFF;
    iocp.presolve = GLP_ON;
    glp_intopt(P, &iocp);

	int temp;
	result_num = 0;

    for(int i=1; i<edge_num+1; i++)
    {
    	temp = glp_mip_col_val(P, i);
    	if(temp == 1)
    	{
			result_num++;
			out_way_path[edge_set[i].start] = edge_set[i].end;
    	}
    }
}

bool judge_result(vector<int> &result)
{
	unordered_set<int> demand_temp = demand_node;
	result.clear();
	result.push_back(start_point);
	while (result.back() != end_point)
	{
		if (demand_temp.find(result.back()) != demand_temp.end())
		{
			demand_temp.erase(result.back());
		}
		result.push_back(out_way_path[result.back()]);
	}

    if(result_num+1 != result.size())
    {
//		subset_del(result, demand_temp);
		while (!demand_temp.empty())
		{
			int kk = 0;
			int k = glp_add_rows(P, 1);
			int start = *(demand_temp.begin());
			demand_temp.erase(start);
			int current = start;
			do
			{
				int next = out_way_path[current];
				demand_temp.erase(next);
				kk++;
				ind[kk] = edge_index[current][next];
				val[kk] = 1;
				current = next;
			} while (current != start);
			glp_set_row_bnds(P, k, GLP_UP, 0, kk - 1);
			glp_set_mat_row(P, k, kk, ind, val);
		}
    	return false;		
    }
    else
    {
    	return true;
    }
}

void subset_del(vector<int> &src, unordered_set<int> &subset)
{
	for (auto it = src.begin(); it != src.end(); ++it)
	{
		subset.erase(*it);
	}
}



