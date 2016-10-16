#include "Global.h"

int  seed = 3133;
long rnd_uni_init = - long(seed);

int NumOfF;		//目标个数
int NumOfNeibor;//邻居数					
int NumOfSub;//子问题个数
vector<float> ReferencePoint;//参考点 
vector<float> ReferencePointMax;//最大值参考点 
vector<float> Pai;//权益值

int Instance	;//范例的序号
int pop;
int example;

int NumOfRoom;//房间数				//
int  NumOfA;//左中心岛				//
int  NumOfB;//右中心岛				//
double WidOfCorridor;//走廊宽		//
double  WidOfH;//房间高度h			//
vector<double> WidOfRoom;//房间宽度	//
vector<double> LimOfRow;//行宽限制
vector<double> LimOfSecRow;//二行宽度限制

vector< vector<int> > Flow;//房间流量
vector< vector<int> > Adj;//房间流量
vector< vector<float> > subReference;	//子问题参考点


vector< vector<int> > knapsack_profit;
vector< vector<int> > knapsack_weight;
vector<int>           knapsack_capacity;

char strFitnessType[1024];

int run_id;