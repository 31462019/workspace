// WeightVector.cpp: implementation of the CWeightVector class.
//
//////////////////////////////////////////////////////////////////////

#include "WeightVector.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWeightVector::CWeightVector()
{
    vector<double> w_vec(vector<double>(NumOfF, 0));
	lambda  = w_vec;
}

CWeightVector::~CWeightVector()
{

}

double CWeightVector::DistanceTo(CWeightVector &weight)
{
    double dist = 0;
	for(int iObj=0; iObj<NumOfF; iObj++)
	{
		double diff = this->lambda[iObj] - weight.lambda[iObj];
	    dist+= diff*diff;
	}
	return sqrt(dist);
}

void CWeightVector::Show()
{
    for(int iObj=0; iObj<NumOfF; iObj++)
	{
	    printf(" %f ", lambda[iObj]);
	}
	printf("\n");
}