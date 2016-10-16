// Individual.cpp: implementation of the CIndividualividual class.
//
//////////////////////////////////////////////////////////////////////

#include "Individual.h"
#include "time.h"
#include "math.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIndividual::CIndividual()
{

	vector<int> RNum_vec(vector<int>(NumOfRoom, 0));
	vector<int> Break_vec(vector<int>(4, 0));
	vector<double> Obj_vec(vector<double>(NumOfF, 0.0));	
	vector< vector<double> > d_mat(NumOfRoom, vector<double>(NumOfRoom, 0.0));
	
	Arrange  = RNum_vec;
	BreakNum = Break_vec;
	Gap = LimOfSecRow; 
	ObjValue = Obj_vec;
	Distance = d_mat;

	beyond_width = 0;
	s1 = 0.01;
	s2 = 20;
}

CIndividual::~CIndividual()
{

}

// randomly initialize solution
void CIndividual::Randomize()
{
	//Arrange
	for(int index = 0; index != NumOfRoom; index++)
    {
    	 Arrange[index]=index;
    }
	random_shuffle(Arrange.begin(),Arrange.end());

	BreakNum[1] = NumOfA;
	BreakNum[2] = NumOfB;
	int tmp = Rnd.GetNumber()*(NumOfRoom - NumOfA -NumOfB - 1) + 1;

	BreakNum[0]= tmp ,BreakNum[3] = NumOfRoom - NumOfA - NumOfB - tmp;

	

}

// Greedy repair method in MOMHLib++ (Andrzej Jaszkiewicz)
void CIndividual::GreedyRepairHeuristic(vector<double>& lambda, char* strFuncType)
{
}


bool CIndividual::IsFeasible()
{
	int i,j;
	double sum=0;
	beyond_width = 0;
	//二行
	sum += LimOfSecRow[0]+LimOfSecRow[1]+LimOfSecRow[2];
	for(j=0;j<BreakNum[1]+BreakNum[2];j++)
		sum += WidOfRoom[Arrange[j+BreakNum[0]]];
	if(sum > LimOfRow[1])
		beyond_width += sum - LimOfRow[1];
	sum=0;
	//一行
	for(j=0;j<BreakNum[0];j++)
		sum += WidOfRoom[Arrange[j]];
	if(sum > LimOfRow[0])
		beyond_width += sum - LimOfRow[0];
	sum = 0;
	//三行
	for(j=0;j<BreakNum[3];j++)
		sum += WidOfRoom[Arrange[j + NumOfRoom - BreakNum[3]]];
	if(sum > LimOfRow[2])
		beyond_width += sum - LimOfRow[2];
	if(beyond_width > 0.0)
		return false;
	return true;

}


bool CIndividual::ComputingDistance()		
{

	vector<double> locate(vector<double>(NumOfRoom, 0.0));
	vector<int> locate_row(vector<int>(NumOfRoom, 0));		
	int tab;

	//the first line
	tab = Arrange[0];
	locate[tab] = WidOfRoom[tab]*0.5;
	locate_row[tab] = 0;
	for (int i = 1; i < BreakNum[0]; ++i)
	{
		tab = Arrange[i];
		locate[tab] = locate[Arrange[i-1]] + WidOfRoom[Arrange[i-1]]*0.5 + WidOfRoom[tab]*0.5;
		locate_row[tab] = 0;
	}

	//the second line
	tab = Arrange[BreakNum[0]];
	locate[tab] = WidOfRoom[tab]*0.5 + Gap[0];
	locate_row[tab] = 1;
	for (int i = BreakNum[0]+1 ; i < BreakNum[0] + BreakNum[1] + BreakNum[2]; ++i)
	{
		tab = Arrange[i];
		locate[tab] = locate[Arrange[i-1]] + WidOfRoom[Arrange[i-1]]*0.5 + WidOfRoom[tab]*0.5;
		locate_row[tab] = 1;
		if (i == BreakNum[0]+BreakNum[1])
			locate[tab] += Gap[1];
	}

	//the third line
	tab = Arrange[BreakNum[0]+BreakNum[1]+BreakNum[2]];
	locate[tab] = WidOfRoom[tab]*0.5;
	locate_row[tab] = 2;
	for (int i = BreakNum[0]+BreakNum[1]+BreakNum[2] + 1; i < NumOfRoom; ++i)
	{
		tab = Arrange[i];
		locate[tab] = locate[Arrange[i-1]] + WidOfRoom[Arrange[i-1]]*0.5 + WidOfRoom[tab]*0.5;
		locate_row[tab] = 2;
	}	


	
	for (int i = 0; i < NumOfRoom; ++i)
	{
		for (int j = i+1; j < NumOfRoom; ++j)		
			Distance[i][j] = fabs(locate[i]-locate[j]) + abs(locate_row[i] - locate_row[j]) * (WidOfCorridor + WidOfH);
	}

	return true;
}

void CIndividual::ComputingObjValue()
{
	ComputingDistance();
	
	ObjValue[0] = 0.0;
	ObjValue[1] = 0.0;
	for (int i = 0; i < NumOfRoom; ++i)
	{
		for (int j = i+1; j < NumOfRoom; ++j)
		{
			ObjValue[0] += Distance[i][j] * Flow[i][j];
			ObjValue[1] += Distance[i][j] * Adj[i][j];
		}
	}
	//printf("%f %f\n", ObjValue[0],ObjValue[1]);
	//ObjValue[0] = ObjValue[0]/1000;
	IsFeasible();

}

bool CIndividual::operator>(const CIndividual& indiv)
{
    for(int i=0;i<NumOfF;i++)
		if(indiv.ObjValue[i]>ObjValue[i])
		    return false;
    return true;
}

bool CIndividual::operator<(const CIndividual& indiv)
{
    for(int i=0;i<NumOfF;i++)
		if(indiv.ObjValue[i]<ObjValue[i])
			return false;
    return true;
}

bool CIndividual::operator==(const CIndividual& ind)
{
	for(int i=0;i<NumOfF;i++)
	   if(ind.ObjValue[i]!=ObjValue[i])
	        return false;
	/*
	for (int i = 0; i < NumOfRoom; ++i)
	{
		if (ind.Arrange[i]!=Arrange[i])
			return false;
	}
	*/
	return true;
}

void CIndividual::Show()
{
	printf("%f,%f",ObjValue[0],ObjValue[1]);

}



double CIndividual::ComputingFitnessValue(vector<double> &lambda, char *strFuncType,double t)
{
	double fitness;
	//IsFeasible();
	if(!strcmp(strFuncType,"_TCHEBYCHEFF")) 
	{
		fitness = -1e30;
		for (int i = 0; i < NumOfF; i++) 
		{
			//double dif = 1.1*ReferencePoint[i] - this->ObjValue[i];
			//double dif = (ReferencePoint[i] - this->ObjValue[i])/ReferencePoint[i];
			
			double distance =  lambda[i]*(this->ObjValue[i] - ReferencePoint[i]);
			if (beyond_width <= t)
				distance += s1 * pow(beyond_width,2);
			else
				distance += s1 * pow(beyond_width,2) + s2*(beyond_width - t);
			//double dif = distance/this->ObjValue[i];
			double dif = distance/(ReferencePointMax[i] - ReferencePoint[i]);
			double s = dif>0?dif:-dif;
			if (s >	fitness) 	fitness = s;
		}		
	}

	return fitness;
}

void CIndividual::SinglePointInsert(CIndividual &parent, vector<double> &lambda, char *strFuncType,double t)		//insert
{
	vector<double> sub_obj = parent.ObjValue;
	double fitness = parent.ComputingFitnessValue(lambda,strFuncType,t);
	//printf("fitness %f \n",fitness);

	//the first row
	int s_a = 0;
	int s_b = 0;
	int tag = 0;
	if( BreakNum[0] > 1){
		for (int i = 0; i < parent.BreakNum[0]; ++i){
			for (int j = 0; j < parent.BreakNum[3] + 1; ++j){
				this->Arrange.erase(Arrange.begin() + i);
				this->Arrange.insert(Arrange.end() - j,parent.Arrange[i]);


				this->BreakNum[0] = this->BreakNum[0] - 1;
				this->BreakNum[3] = this->BreakNum[3] + 1;
			

				this->ComputingObjValue();
				if (this->ComputingFitnessValue(lambda,strFuncType,t) < fitness)
				{
					fitness = this->ComputingFitnessValue(lambda,strFuncType,t);
					s_a = i;s_b = j;
					tag = 1;
				}
				this->Arrange = parent.Arrange;
				this->BreakNum = parent.BreakNum;
			}
		}
	}
	//the third row
	if (BreakNum[3] > 1)
	{
		for (int i = 0; i < parent.BreakNum[3]; ++i){
			for (int j = 0; j < parent.BreakNum[0] + 1; ++j){

				this->Arrange.erase(Arrange.end() - i - 1);
				this->Arrange.insert(Arrange.begin() + j,parent.Arrange[NumOfRoom - i -1]);

				this->BreakNum[0] = this->BreakNum[0] + 1;
				this->BreakNum[3] = this->BreakNum[3] - 1;
				this->ComputingObjValue();
				if (this->ComputingFitnessValue(lambda,strFuncType,t) < fitness)
				{
					fitness = this->ComputingFitnessValue(lambda,strFuncType,t);
					s_a = i;s_b = j;
					tag = 3;
				}
				this->Arrange = parent.Arrange;
				this->BreakNum = parent.BreakNum;

			}
		}
	}
	
	//printf("s_a s_b------ %d %d \n", s_a ,s_b);
	if (tag == 1)
	{
		this->Arrange.erase(Arrange.begin() + s_a);
		this->Arrange.insert(Arrange.end() - s_b,parent.Arrange[s_a]);
		this->BreakNum[0] = this->BreakNum[0] - 1;
		this->BreakNum[3] = this->BreakNum[3] + 1;
		this->ComputingObjValue();
	}
	else if(tag == 3)
	{
		this->Arrange.erase(Arrange.end()- s_a - 1);
		this->Arrange.insert(Arrange.begin() + s_b,parent.Arrange[NumOfRoom - s_a - 1]);
		this->BreakNum[0] = this->BreakNum[0] + 1;
		this->BreakNum[3] = this->BreakNum[3] - 1;
		this->ComputingObjValue();

	}

}

void CIndividual::SinglePointXover(vector<int> &mark ,CIndividual &parent, vector<double> &lambda, char *strFuncType,double t)
{
	//the goal of this function is just compute the best answer in its neibor
	vector<double> sub_obj = parent.ObjValue;
	double fitness = 1;
	int tmp;
	int s_a = 0;
	int s_b = 0;	
	for (int i = 0; i < NumOfRoom - 1; ++i)
	{
		for (int j = i+1; j < NumOfRoom; ++j)
		{
			tmp = this->Arrange[i];
			this->Arrange[i] = this->Arrange[j];
			this->Arrange[j] = tmp; 
			this->ComputingObjValue();
			if (this->ComputingFitnessValue(lambda,strFuncType,t) < fitness){
				fitness = this->ComputingFitnessValue(lambda,strFuncType,t);
				s_a = i;
				s_b = j;
			}
			this->Arrange = parent.Arrange;
		}
	}
	if(Arrange[s_a] < Arrange[s_b]){
		mark[0] = Arrange[s_a];
		mark[1] = Arrange[s_b];
	}
	else{
		mark[0] = Arrange[s_b];
		mark[1] = Arrange[s_a];
	}
	
	tmp = this->Arrange[s_a];
	this->Arrange[s_a] = this->Arrange[s_b];
	this->Arrange[s_b] = tmp;
	this->ComputingObjValue();	
	

}
