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

extern int NumOfF;//Ŀ�����
extern int NumOfNeibor;//�ھ���				
extern int Instance;
extern int pop;
extern int example;

extern int NumOfRoom;//������				//
extern int  NumOfA;//�����ĵ�				//
extern int  NumOfB;//�����ĵ�				//
extern double WidOfCorridor;//���ȿ�		//
extern double  WidOfH;//����߶�h			//
extern vector<double> WidOfRoom;//������	//
extern vector<double> LimOfRow;//�п�����
extern vector<double> LimOfSecRow;//���п������

extern vector< vector<int> > Flow;//��������
extern vector< vector<int> > Adj;//���ڳ̶�
//extern vector< vector<float> > subReference;	//������ο���

//extern char strFitnessType[1024];

//extern int run_id;
	   int NumOfRoom;
	   int NumOfA;
	   int NumOfB;
	   vector< vector<int> > flow;
	   vector< vector<int> > adj;


#endif