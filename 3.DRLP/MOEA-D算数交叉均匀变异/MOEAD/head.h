#ifndef head
#define head

#include "stdio.h"
#include "time.h"
#include "cstdlib"
using namespace std;
const int n = 8; 		//�ܷ�����
const int na = 2;		//�����ĵ��������ĵ�������
const int nb = 2;		//
const int  m = 2;			//Ŀ�꺯������
const int N = 100;
const int T = 10;


int gen=0;			//�Ŵ�����
double ld[N][m];	//Lambdaֵ
//����
int fst[N][n];		//����
int sec[N][4];		//������
float Pai[N];		//Ȩ��ֵ

int B[N][T];		//�ھӼ�


//������������
void randomld();
void init();
void buildB();
#endif