#include "Global.h"

int  seed = 3133;
long rnd_uni_init = - long(seed);

int NumOfF;		//Ŀ�����
int NumOfNeibor;//�ھ���					
int NumOfSub;//���������
vector<float> ReferencePoint;//�ο��� 
vector<float> ReferencePointMax;//���ֵ�ο��� 
vector<float> Pai;//Ȩ��ֵ

int Instance	;//���������
int pop;
int example;

int NumOfRoom;//������				//
int  NumOfA;//�����ĵ�				//
int  NumOfB;//�����ĵ�				//
double WidOfCorridor;//���ȿ�		//
double  WidOfH;//����߶�h			//
vector<double> WidOfRoom;//������	//
vector<double> LimOfRow;//�п�����
vector<double> LimOfSecRow;//���п������

vector< vector<int> > Flow;//��������
vector< vector<int> > Adj;//��������
vector< vector<float> > subReference;	//������ο���


vector< vector<int> > knapsack_profit;
vector< vector<int> > knapsack_weight;
vector<int>           knapsack_capacity;

char strFitnessType[1024];

int run_id;