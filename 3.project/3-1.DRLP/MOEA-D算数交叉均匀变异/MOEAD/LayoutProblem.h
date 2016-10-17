// KnapsackProblem.h: interface for the CLayoutProblem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LAYOUTPROBLEM_H__17239787_D864_451E_9DDE_221B0A75A015__INCLUDED_)
#define AFX_LAYOUTPROBLEM_H__17239787_D864_451E_9DDE_221B0A75A015__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Global.h"

using namespace std;

class CLayoutProblem  
{
public:
	CLayoutProblem();
	virtual ~CLayoutProblem();

public:
	   /*
	   vector<int>    Arrange;     // 房间排列
       vector<int>    BreakNum;    // 切分点
	   vector<double>	alpha;		//二行的间距
	   vector<double>    ObjValue;  //目标函数值
	   */
//       vector< vector<int> > profit;
//       vector< vector<int> > weight;
//       vector<int>           capacity;
	   //int NumOfFs;
	   int NumOfRoom;
	   int NumOfA;
	   int NumOfB;
	   vector< vector<int> > flow;
	   vector< vector<int> > adj;

public:
	   bool LoadInstance(char* Filename);
	   void Initialize(int room,int a,int b);
	   void Show();
};

#endif // !defined(AFX_KNAPSACKPROBLEM_H__17239787_D864_451E_9DDE_221B0A75A015__INCLUDED_)
