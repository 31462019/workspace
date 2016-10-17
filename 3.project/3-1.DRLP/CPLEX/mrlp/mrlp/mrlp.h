#ifndef __MRLP_H_
#define __MRLP_H_



#include <cstdlib>
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>
#include <ctime>
#include <string>
#include <time.h>

#include <iomanip>

using namespace std;

#define Row 3					//


//extern int  seed;
//extern long rnd_uni_init;

extern int NumOfF;//目标个数
extern int NumOfNeibor;//邻居数				
extern int Instance;
extern int pop;
extern int example;

extern int NumOfRoom;//房间数				//
extern int  NumOfA;//左中心岛				//
extern int  NumOfB;//右中心岛				//
extern double WidOfCorridor;//走廊宽		//
extern double  WidOfH;//房间高度h			//
extern vector<double> WidOfRoom;//房间宽度	//
extern vector<double> LimOfRow;//行宽限制
extern vector<double> LimOfSecRow;//二行宽度限制

extern vector< vector<int> > Flow;//房间流量
extern vector< vector<int> > Adj;//紧邻程度
//extern vector< vector<float> > subReference;	//子问题参考点

//extern char strFitnessType[1024];

//extern int run_id;
	   int NumOfRoom;
	   int NumOfA;
	   int NumOfB;
	   vector< vector<int> > flow;
	   vector< vector<int> > adj;


#endif