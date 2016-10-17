#ifndef head
#define head

#include "stdio.h"
#include "time.h"
#include "cstdlib"
using namespace std;
const int n = 8; 		//总房间数
const int na = 2;		//左中心岛和右中心岛房间数
const int nb = 2;		//
const int  m = 2;			//目标函数个数
const int N = 100;
const int T = 10;


int gen=0;			//遗传代数
double ld[N][m];	//Lambda值
//变量
int fst[N][n];		//排序
int sec[N][4];		//房间数
float Pai[N];		//权益值

int B[N][T];		//邻居集


//函数声明部分
void randomld();
void init();
void buildB();
#endif