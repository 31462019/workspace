// Individual.h: interface for the CIndividual class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INDIVIDUAL_H__2BA89BEB_BCB1_491E_8980_761AF58D14BC__INCLUDED_)
#define AFX_INDIVIDUAL_H__2BA89BEB_BCB1_491E_8980_761AF58D14BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Global.h"
#include "LayoutProblem.h"
#include "RandomNumber.h"
#include "algorithm"

using namespace std;

class CIndividual  
{
public:
	CIndividual();
	virtual ~CIndividual();

public:


       vector<int>    Arrange;     // items in knapsack - binary
       vector<int>    BreakNum;    // capacity of knapsacks
	   vector<double>	Gap;
	   vector<double>    ObjValue;    // profits of items in knapack
	   vector< vector<double> > Distance;//存储相互之间的距离

	   
public:
	   CRandomNumber    Rnd;
	   bool             dominated;

	   double			beyond_width;
	   
	   int s1,s2;								//parameter
public:
	   bool   IsFeasible();           //check the constraints是否可行
	   void   ComputingObjValue();		//计算目标函数值

	   void   SinglePointXover(vector<int> &mark ,CIndividual &parent ,vector<double> &lambda, char *strFuncType,double t);
	   void   SinglePointInsert(CIndividual &parent ,vector<double> &lambda, char *strFuncType,double t);
	   double ComputingFitnessValue(vector<double> &lambda, char *strFuncType,double t);
	   void   Randomize();

       void   Show();
       bool   ComputingDistance();        // capacity computing

       void   GreedyRepairHeuristic(vector<double> &lambda, char* strFuncType);

  // 	   bool   IsFeasible();           //check the constraints是否可行


       bool operator==(const CIndividual& ind);
       bool operator>(const  CIndividual& ind);
	   bool operator<(const  CIndividual& ind);

};

#endif // !defined(AFX_INDIVIDUAL_H__2BA89BEB_BCB1_491E_8980_761AF58D14BC__INCLUDED_)
