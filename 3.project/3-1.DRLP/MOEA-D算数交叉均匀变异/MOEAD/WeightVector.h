// WeightVector.h: interface for the CWeightVector class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WEIGHTVECTOR_H__509BD044_BCAB_4FE0_90F6_B120691B98BA__INCLUDED_)
#define AFX_WEIGHTVECTOR_H__509BD044_BCAB_4FE0_90F6_B120691B98BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Global.h"

using namespace std;

class CWeightVector  
{
public:
	CWeightVector();
	virtual ~CWeightVector();

	void Show();

public:
	vector<double> lambda;

public:
	double DistanceTo(CWeightVector &weight);
};

#endif // !defined(AFX_WEIGHTVECTOR_H__509BD044_BCAB_4FE0_90F6_B120691B98BA__INCLUDED_)
