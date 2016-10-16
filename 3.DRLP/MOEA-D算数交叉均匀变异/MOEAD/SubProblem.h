// SubProblem.h: interface for the CSubProblem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUBPROBLEM_H__F16FA62A_D0C0_40A5_B1EC_91ED537AACD3__INCLUDED_)
#define AFX_SUBPROBLEM_H__F16FA62A_D0C0_40A5_B1EC_91ED537AACD3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Individual.h"
#include "WeightVector.h"

class CSubProblem  
{
public:
	CSubProblem();
	virtual ~CSubProblem();

public:
	CIndividual    CurrentSolution;
	CWeightVector  WeightVector;
	vector<int>    IndexOfNeighbor;
};

#endif // !defined(AFX_SUBPROBLEM_H__F16FA62A_D0C0_40A5_B1EC_91ED537AACD3__INCLUDED_)
