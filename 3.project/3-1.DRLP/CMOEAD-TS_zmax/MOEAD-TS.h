// MOEAD.h: interface for the CMOEAD class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOEAD_H__968492DD_9800_4780_ABA8_6F78D3D1491E__INCLUDED_)
#define AFX_MOEAD_H__968492DD_9800_4780_ABA8_6F78D3D1491E__INCLUDED_

#include "RandomNumber.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include  "Global.h"
#include  "Individual.h"
#include  "WeightVector.h"
#include  "SubProblem.h"
#include "ContinueValue.h"



class CMOEAD  
{
public:
	CMOEAD();
	virtual ~CMOEAD();


public:
	vector<CSubProblem> CurrentPopulation;  //  population of subproblems
	vector<CIndividual> SecondPopulation;   //  external population containing all nondominated solutions found 
	vector< vector<int> > TabuList;
	

public:
	int PopulationSize;                     //  population size
	int MaxNumOfFuncEvals;                  //  the maximal number of function evaluations
	int NeighborhoodSize;                   //  neighborhood size
	int NumberOfFuncEvals;                  //  counter for function evaluations 目标函数值的计数器
	CRandomNumber Rnd;                      //  uniform random number generator
	int lenthTabu;

	int count_nonupdate;

	//constrainted information
	
	double maxV,minV;						//max min value
	double t;								// tao

public:
	void SaveSecondPopulation();
	void Show();
	void MinFastSort(vector<double> &dist, vector<int> &indx, int n, int m);
	void MaxFastSort(vector<double> &dist, vector<int> &indx, int n, int m);
	int TornamentSelection();
	

	void InitializePopulation();
	void InitializeWeightVector();
	void InitializeNeighborhood();
	void InitializeReferencePoint();

	void UpdateReferencePoint(CIndividual &ind);
	void UpdateSecondPopulation(CIndividual &ind);
	void UpdateNeighboringSolution(CIndividual &ind, int iPop);

	void OptimizeSecondPopulation();

	void Run(int fevals);

	void PartiallyMatchedCrossover(CIndividual parent1,CIndividual parent2, CIndividual &child1,  CIndividual &child2);
	void Mutation(CIndividual parent,CIndividual &child);
	void CycleCrossover(CIndividual parent1,CIndividual parent2, CIndividual &child1,  CIndividual &child2);

	void evolution();

	//Tabu Search related
	void UpdateSubReference();
	void TabuSearch();
	void UpdateTabuList(vector<int> &mark);

	//constrained function
	void OptimizeV();
};


#endif // !defined(AFX_MOEAD_H__968492DD_9800_4780_ABA8_6F78D3D1491E__INCLUDED_)
