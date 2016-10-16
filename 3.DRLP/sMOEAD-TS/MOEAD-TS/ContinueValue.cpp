// -------------------------------------------------------------- -*- C++ -*-
// File: blend.cpp
// Version 12.4  
// --------------------------------------------------------------------------
// Licensed Materials - Property of IBM
// 5725-A06 5725-A29 5724-Y48 5724-Y49 5724-Y54 5724-Y55
// Copyright IBM Corporation 2000, 2011. All Rights Reserved.
//
// US Government Users Restricted Rights - Use, duplication or
// disclosure restricted by GSA ADP Schedule Contract with
// IBM Corp.
// --------------------------------------------------------------------------
//
// blend.cpp -- A blending problem

/* ------------------------------------------------------------

Problem Description
-------------------

Goal is to blend four sources to produce an alloy: pure metal, raw
materials, scrap, and ingots.

Each source has a cost.
Each source is made up of elements in different proportions.
Alloy has minimum and maximum proportion of each element.

Minimize cost of producing a requested quantity of alloy.

------------------------------------------------------------ */
#include <ilcplex/ilocplex.h>
//#include <concert.dll>
#include <math.h>
#include <stdio.h>
#include <iostream>
typedef IloArray<IloNumVarArray> IloNumVarArray2;
typedef IloArray<IloBoolVarArray> IloBoolVarArray2;
typedef IloArray<IloIntVarArray> IloIntVarArray2;
typedef IloArray<IloArray<IloBoolVarArray>> IloBoolVarArray3;

#include "ContinueValue.h"

//确定两个变量，传入一个子问题，返回该子问题连续的最优值
ILOSTLBEGIN

IloIntArray arrange,break_num,locate_row;
IloNumArray locate;
IloNumVarArray gap;


void ContinueValue(CIndividual& ind ,vector<double> &lambda) {
   IloEnv env;
   
   try {
      IloInt i,j,r,n,i1;
	  
	  IloModel model(env);

	 

//--------------------------------------------------------
	  IloInt tab;
	  IloIntArray arrange(env,NumOfRoom);
	  IloIntArray break_num(env,4);
	  IloIntArray locate_row(env,NumOfRoom);
	  IloNumArray locate(env,NumOfRoom);

	  for(int i=0;i<NumOfRoom;i++)
		  arrange[i] = ind.Arrange[i];
	  for(int i = 0;i<4;i++)
		  break_num[i] = ind.BreakNum[i];
	  for(int i=0;i<break_num[0];i++){
		  locate_row[arrange[i]] = 0;
	  }
	
	  //the first line
	  tab = arrange[0];
	  locate[tab] = WidOfRoom[tab]*0.5;
	  locate_row[tab] = 0;
	  for (int i = 1; i < break_num[0]; ++i)
	  {
		  tab = arrange[i];
		  locate[tab] = locate[arrange[i-1]] + WidOfRoom[arrange[i-1]]*0.5 + WidOfRoom[tab]*0.5;
		  locate_row[tab] = 0;
	  }

	  //the second line
	  tab = arrange[break_num[0]];
	  locate[tab] = WidOfRoom[tab]*0.5;
	  locate_row[tab] = 1;
	  for (int i = break_num[0]+1; i < break_num[0] + break_num[1] + break_num[2]; ++i)
	  {
		  tab = arrange[i];
		  locate[tab] = locate[arrange[i-1]] + WidOfRoom[arrange[i-1]]*0.5 + WidOfRoom[tab]*0.5;
		  locate_row[tab] = 1;
	  }

	  //the third line
	  tab = arrange[break_num[0]+break_num[1]+break_num[2]];
	  locate[tab] = WidOfRoom[tab]*0.5;
	  locate_row[tab] = 2;
	  for (int i = break_num[0]+break_num[1]+break_num[2] + 1; i < NumOfRoom; ++i)
	  {
		  tab = arrange[i];
		  locate[tab] = locate[arrange[i-1]] + WidOfRoom[arrange[i-1]]*0.5 + WidOfRoom[tab]*0.5;
	      locate_row[tab] = 2;
	  }	
	  //------------------------------------------------------------------------------------------------
	  //constraints 1
	  //行宽限制
	  IloNumVarArray gap(env, 2, 0.0, 20.0);
	  IloNumVarArray position(env,NumOfRoom,0.0, IloInfinity);
	  IloNum sum1=0;
	  
	  for(i=0;i<ind.BreakNum[1]+ind.BreakNum[2];i++){
		  sum1 += WidOfRoom[arrange[i+break_num[0]]];
	  }
	  model.add(LimOfRow[1] - sum1 - IloSum(gap) >= LimOfSecRow[2]);
	  

	  //constraints 2
	  //存储距离的值
	   //IloNumExpr sum1(env);
	  for(i=0;i<NumOfRoom;i++){
		  IloNumExpr s1(env);
		  if(i>=break_num[0])
			  s1 += gap[0];
		  if(i>=break_num[0]+break_num[1])
			  s1 += gap[1];
		  j = arrange[i];
		  model.add(position[j] == s1 + locate[j]);
		  s1.end();
	  }

	   //constraints 3
	  //宽度要大于最小值
	  for(i=0;i<2;i++)
		  model.add(gap[i]>=LimOfSecRow[i]);




	  //function
	  sum1=0;
	  IloNum   sum2 = 0;
	  IloNumExpr fun1(env),fun2(env);
	  for(i=0;i<NumOfRoom-1;i++){
		  for(j=i+1;j<NumOfRoom;j++){
			  //if(i==j) continue;
			  IloNumExpr distance;
			  IloExpr s1,s2;
			  if(locate_row[arrange[i]]-locate_row[arrange[j]]>0)
				  sum1 = (locate_row[arrange[i]]-locate_row[arrange[j]])*(WidOfCorridor + WidOfH);
			  else
				  sum1 = (locate_row[arrange[i]]-locate_row[arrange[j]])*(WidOfCorridor + WidOfH);
			  distance = sum1 + IloAbs(position[arrange[i]]-position[arrange[j]]);
			  fun1 += distance*Flow[arrange[i]][arrange[j]];
			  fun2 += distance*Adj[arrange[i]][arrange[j]];
			  s1.end();
			  s2.end();
			 
		  }
	  }
	 
	

	  model.add(IloMinimize(env,lambda[0]*fun1+lambda[1]*fun2));
	  fun1.end();
	  fun2.end();

	  IloCplex cplex(env);
	  // cplex.setParam(IloCplex::NodeFileInd,3);
	  
	  cplex.setParam(IloCplex::NodeFileInd,3);
		cplex.setParam(IloCplex::MemoryEmphasis,true);
		cplex.setParam(IloCplex::WorkMem,1024);
		cplex.setParam(IloCplex::TreLim,1024);
		cplex.setParam(IloCplex::Threads,1) ;  
		cplex.setParam(IloCplex::VarSel,3);
		cplex.setOut(env.getNullStream());
      cplex.setWarning(env.getNullStream());
		/*
	  cplex.setParam (IloCplex::WorkMem ,2000);
	  cplex.setParam(IloCplex::NodeFileInd ,3);
	  cplex.setParam (IloCplex::Threads ,1);
	  */
	  cplex.extract(model);
	  
	  cplex.solve();



	  if(cplex.getStatus() == IloAlgorithm::Infeasible)
         env.out() << "No Solution" << endl;
	  
	 


	  
	 
	  //output
//	  IloTimer timer(env);
//	  timer.start();
   //   cplex.solve();	  
//	  timer.stop();
	 // cout<<"time is:"<<timer.getTime()<<endl;

	  ind.Gap[0]=cplex.getValue(gap[0]);
	  ind.Gap[1]=cplex.getValue(gap[1]);
	  ind.ComputingObjValue();
/*	  if(cplex.solve())
      cout << " Minimize cost = " << cplex.getObjValue() << endl<<endl;
	  }*/
	  }
	  catch (IloException& ex) {
		  cerr << "Error: " << ex << endl;
		  cerr<<ex.getMessage()<<endl;
	  }
	  catch (...) {
		  cerr << "Error" << endl;
	  }
	  //IloInt aaa;cin>>aaa;
	  env.end();
	 
}