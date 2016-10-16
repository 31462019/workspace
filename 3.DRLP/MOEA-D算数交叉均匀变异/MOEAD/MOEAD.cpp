// MOEAD.cpp: implementation of the CMOEAD class.
//
//////////////////////////////////////////////////////////////////////

#include "MOEAD-TS.h"
#include "Global.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMOEAD::CMOEAD()
{
    NeighborhoodSize    = 10;
	NumberOfFuncEvals   = 0;	
	NumOfSub			= 100;
	NumOfF 				= 2;
	lenthTabu = 3;
	ReferencePoint = vector<float>(NumOfF, 1e30);//
	ReferencePointMax = vector<float>(NumOfF, -1e30);
	Pai = vector<float>(NumOfSub, 1.0);//

	seed         =  (seed + 3111)%77559; //(unsigned)time(NULL); 
	rnd_uni_init = -(long)seed;    

	vector< vector<float> > s_r(NumOfSub, vector<float>(NumOfF, 0.0));
	subReference = s_r;
     
	limit_time = 18000;

	maxV = -1e30 ;
	minV = 1e30;
}

CMOEAD::~CMOEAD()
{

}

// read predefined weight vectors from txt files
void CMOEAD::InitializeWeightVector()
{
	char filename[1024];
	sprintf(filename,"weights/weight_%d_%d.data", NumOfSub, NumOfF);//filename
	printf("%s\n", filename);
	ifstream indata;//ifstreamÎÄ¼þÊäÈëÁ÷
    indata.open(filename); // opens the file
    if(!indata) { // file couldn't be opened
      cerr << "Error: file could not be opened" << endl;
      exit(1);
	}

	CSubProblem  subp;
	for(int i=1; i<=NumOfSub; i++)
	{
		for(int iKnap=0; iKnap<NumOfF; iKnap++)
		{		
			indata>>subp.WeightVector.lambda[iKnap];
		}
		CurrentPopulation.push_back(subp); //CurrentPopulation
	}
	
	indata.close();
}

// initialize population with N solutions
void CMOEAD::InitializePopulation()
{
    for(int iPop=0; iPop<CurrentPopulation.size(); iPop++)  //CurrentPopulation
	{
	    CurrentPopulation[iPop].CurrentSolution.Randomize();// Randomize()  randomly initialize solution				
		
	    CurrentPopulation[iPop].CurrentSolution.ComputingObjValue();
		//CurrentPopulation[iPop].CurrentSolution.Show();
		UpdateReferencePoint(CurrentPopulation[iPop].CurrentSolution);
		//UpdateReferencePoint(CurrentPopulation[iPop].CurrentSolution);
		for (int i = 0; i < NumOfF; ++i)
		{
			subReference[iPop][i] = CurrentPopulation[iPop].CurrentSolution.ObjValue[i];
		}

		//NumberOfFuncEvals++;
	}
}


// compare the offspring solution with its neighhoring solutions
void CMOEAD::UpdateNeighboringSolution(CIndividual &offspring, int iPop)
{
    for(int n=0; n<NeighborhoodSize; n++)
	{
	    double f1, f2;
		int id = CurrentPopulation[iPop].IndexOfNeighbor[n];    // the index of neighboring subproblem  id
		f1 = offspring.ComputingFitnessValue(CurrentPopulation[id].WeightVector.lambda, strFitnessType);  // fitness of the offspring
		f2 = CurrentPopulation[id].CurrentSolution.ComputingFitnessValue(CurrentPopulation[id].WeightVector.lambda, strFitnessType);  // fitness of neighbors
		// if offspring is better, then update the neighbor
		if(f1<f2)
		{
		    CurrentPopulation[id].CurrentSolution = offspring;
		}
	}
}//

// update the reference point with the best value for each objective
void CMOEAD::UpdateReferencePoint(CIndividual &ind)
{
    for(int iObj=0; iObj<NumOfF; iObj++)
	{
		if(ind.ObjValue[iObj] < ReferencePoint[iObj])
		{
			ReferencePoint[iObj] = ind.ObjValue[iObj];
		}
		if(ind.ObjValue[iObj] > ReferencePointMax[iObj])
		{
			ReferencePointMax[iObj] = ind.ObjValue[iObj];
		}

	}
}


// determine the neighboring relationship between subproblems according to 
// the distances between weight vectors
void CMOEAD::InitializeNeighborhood()
{

    for(int iPop=0; iPop<NumOfSub; iPop++)
    {
	   vector<int>    indx;
	   vector<double> dist;

       for(int iPop2=0; iPop2<NumOfSub; iPop2++)
	   {
		   indx.push_back(iPop2);
		   double tp = CurrentPopulation[iPop].WeightVector.DistanceTo(CurrentPopulation[iPop2].WeightVector);
		   dist.push_back(tp);
	   }
	   
	   this->MinFastSort(dist, indx, NumOfSub, NeighborhoodSize+1);


	   for(int i=0; i<NeighborhoodSize+1; i++)
	   {
	       CurrentPopulation[iPop].IndexOfNeighbor.push_back(indx[i]);
	   }

	   indx.clear();
	   dist.clear();
   }
}

void CMOEAD::InitializeReferencePoint()
{

	vector<double> lambda(vector<double>(NumOfF, 0)); //lambda

	for(int iObj=0; iObj<NumOfF; iObj++)
	{
	    lambda[iObj] = 1.0;
		CIndividual ind;

		ind.Randomize(); 

		
		ind.ComputingObjValue();

		this->UpdateReferencePoint(ind);
		lambda[iObj] = 0.0;
	}
}

void CMOEAD::MinFastSort(vector<double> &dist, vector<int> &indx, int n, int m)
{
	for(int i=0; i<m; i++)
	{
		for(int j=i+1;j<n; j++)
		{
			if(dist[i]>dist[j])
			{
				double temp = dist[i]; 
				dist[i]     = dist[j]; 
				dist[j]		= temp;
				int id      = indx[i];
				indx[i]     = indx[j];
				indx[j]		= id;
			}
		}
	}
}
void CMOEAD::MaxFastSort(vector<double> &dist, vector<int> &indx, int n, int m)
{
	for(int i=0; i<m; i++)
	{
		for(int j=i+1;j<n; j++)
		{
			if(dist[i]<dist[j])
			{
				double temp = dist[i]; 
				dist[i]     = dist[j]; 
				dist[j]		= temp;
				int id      = indx[i];
				indx[i]     = indx[j];
				indx[j]		= id;
			}
		}
	}
}


void CMOEAD::UpdateSecondPopulation(CIndividual &ind)
{
	
	int iCount = 0;
	int n;
	if(!ind.IsFeasible()){
	//	printf("not good！\n");
		return;
	}
    for( n=0; n<SecondPopulation.size(); n++){
		if(ind==SecondPopulation[n])
			return;
		if(SecondPopulation[n]<ind)//
			return;
		if(ind<SecondPopulation[n]){
			SecondPopulation[n].dominated = true;
			iCount++;
		}
		else{
			SecondPopulation[n].dominated = false;
		}
	}
	for(n=0; n<SecondPopulation.size(); n++){
		if(SecondPopulation[n].dominated){
			SecondPopulation.erase(SecondPopulation.begin()+n);
			n--;
		}
	}
	SecondPopulation.push_back(ind);    
	if(SecondPopulation.size()>=3000){
		int tag;
		for(n=1; n<SecondPopulation.size(); n++){
			if(SecondPopulation[n].fitness > SecondPopulation[n-1].fitness)
				tag = n;
		}
		SecondPopulation.erase(SecondPopulation.begin()+tag);
	}
	/*
	//通过当前最优间距优化所有子问题最优间距
	CIndividual current;
	for(n = 0;n < NumOfSub; n++){
		current = CurrentPopulation[n].CurrentSolution;
		current.Gap = ind.Gap;
		current.ComputingObjValue();
		if(current.ComputingFitnessValue(CurrentPopulation[n].WeightVector.lambda, strFitnessType)<CurrentPopulation[n].CurrentSolution.ComputingFitnessValue(CurrentPopulation[n].WeightVector.lambda, strFitnessType)){
			CurrentPopulation[n].CurrentSolution = current;
			UpdateReferencePoint(current);
			UpdateSecondPopulation(current);
			UpdateNeighboringSolution(current, n);
		}
	}
	*/
}

void CMOEAD::SaveSecondPopulation()
{
	char saveFilename[1024];
	sprintf(saveFilename,"POF/Room%d_Pop%d_Instance%d_example%d.data", NumOfRoom, pop, Instance, example);
	std::fstream fout;
	fout.open(saveFilename,std::ios::out);
	fout<<SecondPopulation.size()<<endl;
	fout.precision(12);
	fout<<"--------------------------------\n";
	for(int n=0; n<SecondPopulation.size()-1; n++){
		fout<<SecondPopulation[n].ObjValue[0]<<",";
	}
	fout<<SecondPopulation[SecondPopulation.size()-1].ObjValue[0]<<endl;

	fout<<endl;
	for(int n=0; n<SecondPopulation.size()-1; n++){
		fout<<SecondPopulation[n].ObjValue[1]<<",";
	}
	fout<<SecondPopulation[SecondPopulation.size()-1].ObjValue[1]<<endl;
	fout<<"--------------------------------\n";
	for(int n=0;n<SecondPopulation.size();n++){
		for(int k = 0;k < NumOfRoom;k++)
			fout<<SecondPopulation[n].Arrange[k]<<" ";
		fout<<"\t";
		for(int k = 0;k < 4;k++)
			fout<<SecondPopulation[n].BreakNum[k]<<" ";
		fout<<"\t";
		for(int k = 0;k < 2;k++)
			fout<<SecondPopulation[n].Gap[k]<<" ";
		fout<<"\n";
	}
	fout.close();
}

void CMOEAD::OptimizeSecondPopulation()
{
	//对同一组合下不同lambda值进行比较
	int before = SecondPopulation.size();
	
	int n,m;
    for( n=0; n<SecondPopulation.size(); n++){
		CIndividual ind = SecondPopulation[n];
		
		for(m=0;m<NumOfSub;m++){
			//ContinueValue(ind ,CurrentPopulation[m].WeightVector.lambda);
			UpdateReferencePoint(ind);
			UpdateSecondPopulation(ind);
		}
		//ContinueValue(ind ,CurrentPopulation[NumOfSub-1].WeightVector.lambda);
		//UpdateReferencePoint(ind);
		//UpdateSecondPopulation(ind);
	}
	int after = SecondPopulation.size();
	printf("After optimization ,EP adds %d solutions \n",after-before);
}

void CMOEAD::Run(int fevals)
{
	time_t  begin,end;
	begin = clock();
	end = clock();
	
	double current = (double)(end - begin)/CLOCKS_PER_SEC;
	
	//printf("---------------------------------------------------");
	this->InitializeReferencePoint();
	//printf("init reference done!\n");

    this->InitializeWeightVector();
    //printf("init WeightVector done!\n");
	
	this->InitializeNeighborhood();
	//printf("init neighbore done!\n");
	
	this->InitializePopulation();
	//printf("init population done!\n");
	OptimizeV();
	int   gen = 0;
	//printf("current %f limit_time %f",current,limit_time);
	while(current <  limit_time){
		//printf("gen = %d !!!population size %d!!!ReferencePoint %f %f\n",gen,SecondPopulation.size(),ReferencePoint[0],ReferencePoint[1]);

		end = clock();
		current = (double)(end - begin)/CLOCKS_PER_SEC;
		//printf("current %f",current);
		gen++;
		
		evolution();

		NumberOfFuncEvals++;		
		OptimizeV();
	    if(current >=  limit_time)
		{
			printf("\n run %d %d solutions found by MOEAD gen = %d\n", run_id, SecondPopulation.size(),gen);
			break;
		}
		//if(gen %1000 == 0)
			//printf("gen%d size %d Refer %f %f\n",gen,SecondPopulation.size(),ReferencePoint[0],ReferencePoint[1]);
	}
	//OptimizeSecondPopulation();
	//printf("\n run %d %d nondominated solutions found by MOEAD \n", run_id, SecondPopulation.size());

	printf("++++++++++++++++++++++++++++++++++++++++++\n");
	printf("%f %f\n", ReferencePoint[0],ReferencePoint[1]);
	printf("++++++++++++++++++++++++++++++++++++++++++\n");
	SaveSecondPopulation();
	//this->Show();
	
}

void CMOEAD::OptimizeV()
{
	maxV = -1e30,minV = 1e30;

	for(int i = 0;i < NumOfSub;i++){
		if (CurrentPopulation[i].CurrentSolution.beyond_width > maxV )
			maxV = CurrentPopulation[i].CurrentSolution.beyond_width;
		if (CurrentPopulation[i].CurrentSolution.beyond_width < minV )
			minV = CurrentPopulation[i].CurrentSolution.beyond_width;
	}
	t = minV + 0.3*(maxV - minV);
 	
}
void CMOEAD::evolution()
{
	for(int iPop=0; iPop < NumOfSub; iPop++){
		int id1 = int(Rnd.GetNumber()*NeighborhoodSize); 
		int id2 = int(Rnd.GetNumber()*NeighborhoodSize); 
		int p1 = CurrentPopulation[iPop].IndexOfNeighbor[id1];
		int p2 = CurrentPopulation[iPop].IndexOfNeighbor[id2];
		// produce two offspring solution
		CIndividual offspring1 = CurrentPopulation[p1].CurrentSolution;
		CIndividual offspring2 = CurrentPopulation[p2].CurrentSolution;
		//printf("%d %d\n",offspring1.BreakNum[0],offspring2.BreakNum[3] );
		double tmp = Rnd.GetNumber();
		if(tmp<0.4){
			Mutation(CurrentPopulation[p1].CurrentSolution, offspring1);
			Mutation(CurrentPopulation[p2].CurrentSolution, offspring2);
		}
		else if(tmp > 0.7)
			//CycleCrossover(CurrentPopulation[p1].CurrentSolution,CurrentPopulation[p2].CurrentSolution,offspring1,offspring2);
			PartiallyMatchedCrossover(CurrentPopulation[p1].CurrentSolution,CurrentPopulation[p2].CurrentSolution,offspring1,offspring2);
		else
			CycleCrossover(CurrentPopulation[p1].CurrentSolution,CurrentPopulation[p2].CurrentSolution,offspring1,offspring2);
		
		if(offspring2.ComputingFitnessValue(CurrentPopulation[iPop].WeightVector.lambda, strFitnessType) < offspring1.ComputingFitnessValue(CurrentPopulation[iPop].WeightVector.lambda, strFitnessType)){
		//if(offspring2.ObjValue[0] <= offspring1.ObjValue[0] && offspring2.ObjValue[1] <= offspring1.ObjValue[1]){
			CurrentPopulation[iPop].CurrentSolution = offspring2;
			UpdateReferencePoint(offspring2);
			UpdateSecondPopulation(offspring2);
			UpdateNeighboringSolution(offspring2, iPop);
			CIndividual offspring2_2 = offspring2;
			double tmp = Rnd.GetNumber();
			if(tmp<0.01)
			{	
					continueMutation(offspring2,offspring2_2);
					//ContinueValue(offspring2,CurrentPopulation[iPop].WeightVector.lambda);
					
			}
			else if(tmp<0.71){
				continueArithmeticCrossover(offspring2,offspring2_2);

			}
			if(offspring2_2.ComputingFitnessValue(CurrentPopulation[iPop].WeightVector.lambda, strFitnessType) > offspring2.ComputingFitnessValue(CurrentPopulation[iPop].WeightVector.lambda, strFitnessType))
				offspring2_2 = offspring2;
				
				CurrentPopulation[iPop].CurrentSolution = offspring2_2;
				UpdateReferencePoint(offspring2_2);
				UpdateSecondPopulation(offspring2_2);
				UpdateNeighboringSolution(offspring2_2, iPop);
		}
		else{
			CurrentPopulation[iPop].CurrentSolution = offspring1;
			UpdateReferencePoint(offspring1);
			UpdateSecondPopulation(offspring1);
			UpdateNeighboringSolution(offspring1, iPop);
			CIndividual offspring1_1 = offspring1;
			double tmp = Rnd.GetNumber();
			if(tmp<0.01)
			{	
				continueMutation(offspring1,offspring1_1);
				//ContinueValue(offspring2,CurrentPopulation[iPop].WeightVector.lambda);

			}
			else if(tmp<0.71){
				continueArithmeticCrossover(offspring1,offspring1_1);
			}
			if(offspring1_1.ComputingFitnessValue(CurrentPopulation[iPop].WeightVector.lambda, strFitnessType) > offspring1.ComputingFitnessValue(CurrentPopulation[iPop].WeightVector.lambda, strFitnessType))
				offspring1_1 = offspring1;
			CurrentPopulation[iPop].CurrentSolution = offspring1_1;
			UpdateReferencePoint(offspring1_1);
			UpdateSecondPopulation(offspring1_1);
			UpdateNeighboringSolution(offspring1_1, iPop);
		}
			

		
		
		//NumberOfFuncEvals++;						
	    //if(NumberOfFuncEvals>=MaxNumOfFuncEvals) break;
	}

}


void CMOEAD::continueArithmeticCrossover(CIndividual parent,CIndividual &child)
{
	double rand = Rnd.GetNumber();
	//double rand = 0.5;

	child.Gap[0] = rand*parent.Gap[0]+(1-rand)*parent.Gap[1];
    child.Gap[1] = (1-rand)*parent.Gap[0]+rand*parent.Gap[1];

	//printf("%f %f",child.Gap[0],child.Gap[1]);
    /*
	while(child.Gap[0]<LimOfSecRow[0] || child.Gap[1]<LimOfSecRow[1])
	{
		rand = Rnd.GetNumber();
		child.Gap[0] = rand*parent.Gap[0]+(1-rand)*parent.Gap[1];
		child.Gap[1] = (1-rand)*parent.Gap[0]+rand*parent.Gap[1];
	}*/
    child.ComputingObjValue();
			


}

void CMOEAD::continueMutation(CIndividual parent,CIndividual &child)
{
	double Umax = LimOfRow[1] - LimOfSecRow[0] - LimOfSecRow[1] - LimOfSecRow[2];
	for(int i =0;i<parent.BreakNum[1]+parent.BreakNum[2];i++){
		Umax -= WidOfRoom[parent.Arrange[i+parent.BreakNum[0]]];
	}
	if(Umax <LimOfSecRow[0]  + LimOfSecRow[1]){

		child.Gap[0]<LimOfSecRow[0];
		child.Gap[1]<LimOfSecRow[1];
		return;
	}
		
	
	child.Gap[0] = LimOfSecRow[0] +Rnd.GetNumber()*(Umax - LimOfSecRow[0]);
	child.Gap[1] = LimOfSecRow[1] +Rnd.GetNumber()*(Umax - LimOfSecRow[1]);

    child.ComputingObjValue();
}

void CMOEAD::PartiallyMatchedCrossover(CIndividual parent1,CIndividual parent2, CIndividual &child1,  CIndividual &child2)
{

	int tmp,num1,num2,j;
	//Random select two point
    int k1 = Rnd.GetNumber()*NumOfRoom;
	int k2 = Rnd.GetNumber()*NumOfRoom;
	if (k1 == k2)
		k2 = (k2 + 1) % NumOfRoom;
	if (k1 > k2 ){
		tmp = k1;k1 = k2;k2 = tmp;
	}	
	//printf("%d %d\n", k1,k2);
	for (int i = k1; i <= k2; ++i){
		child1.Arrange[i] = parent2.Arrange[i];
		child2.Arrange[i] = parent1.Arrange[i];
	}
	
	for (int i = 0; i < NumOfRoom; ++i)
	{
		if (i>=k1 && i<=k2)
			continue;
		num1 = parent1.Arrange[i]; num2 = parent2.Arrange[i];

		vector<int>::iterator result_iter = find(parent1.Arrange.begin() + k1,parent1.Arrange.begin()+k2+1, num2);
		while(result_iter != parent1.Arrange.begin() + k2 + 1){
			
			j = k1;
			while(parent1.Arrange[j] != num2)	j +=1;
			num2 = parent2.Arrange[j];
			result_iter = find(parent1.Arrange.begin() + k1,parent1.Arrange.begin()+k2+1, num2);
		}
		child2.Arrange[i] = num2;
		
		result_iter = find(parent2.Arrange.begin() + k1,parent2.Arrange.begin()+k2+1, num1);
		while(result_iter != parent2.Arrange.begin() + k2 + 1){
			
			j = k1;
			while(parent2.Arrange[j] != num1)	j +=1;
			num1 = parent1.Arrange[j];
			result_iter = find(parent2.Arrange.begin() + k1,parent2.Arrange.begin()+k2+1, num1);
		}
		child1.Arrange[i] = num1;
		
	}

	child1.ComputingObjValue();
	child2.ComputingObjValue();
}

void CMOEAD::Mutation(CIndividual parent,CIndividual &child)
{
	int k1 = Rnd.GetNumber()*NumOfRoom;
	int k2 = Rnd.GetNumber()*NumOfRoom;
	if(k1 == k2)
		k2 = (k2 + 1) % NumOfRoom;
    child.Arrange[k1] = parent.Arrange[k2];
    child.Arrange[k2] = parent.Arrange[k1];
    
    child.ComputingObjValue();
}


void CMOEAD::CycleCrossover(CIndividual parent1,CIndividual parent2, CIndividual &child1,  CIndividual &child2)
{
	vector<int> mark(vector<int>(NumOfRoom, 0));
	
	int count = 0;
	int tmp;
	for (int i = 0; i < NumOfRoom; ++i)
	{
		if (mark[i] == 1)continue;
		if (count % 2 == 1)
		{
			count += 1;
			int j = i;
			child1.Arrange[i] = parent2.Arrange[i];
			child2.Arrange[i] = parent1.Arrange[i];
			mark[i] = 1;

			while(parent2.Arrange[j] != parent1.Arrange[i]){
				
				tmp = parent2.Arrange[j]; 
				j = 0;

				while(parent1.Arrange[j] != tmp)	j +=1;
				child1.Arrange[j] = parent2.Arrange[j];
				child2.Arrange[j] = parent1.Arrange[j];
				
				mark[j] = 1;
				
			}
		}
		else{
			count += 1;
			int j = i;
			mark[i] = 1;

			while(parent1.Arrange[j] != parent2.Arrange[i]){
				tmp = parent1.Arrange[j]; 
				
				j = 0;
				while(parent2.Arrange[j] != tmp)	j +=1;
				mark[j] = 1;	
			}
		}
	}

	child1.ComputingObjValue();
	child2.ComputingObjValue();
}



void CMOEAD::Show()
{
	printf("%f",SecondPopulation[0].ObjValue[0]);
    for(int iPop=1; iPop<SecondPopulation.size(); iPop++)
	{
		printf(",%f",SecondPopulation[iPop].ObjValue[0]);
	}
	printf("\n");
	printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
	printf("%f",SecondPopulation[0].ObjValue[1]);
    for(int iPop=1; iPop<SecondPopulation.size(); iPop++)
	{
		printf(",%f",SecondPopulation[iPop].ObjValue[1]);
	}
	printf("\n");
	/*
	for(int iPop=1; iPop<SecondPopulation.size(); iPop++)
	{
		printf("%f %f ",SecondPopulation[iPop].ObjValue[0],SecondPopulation[iPop].ObjValue[1]);
		printf("%%%%%%%%%%%%");
		for(int i = 0; i<NumOfRoom;i++)
			printf("%d ",SecondPopulation[iPop].Arrange[i]);
		printf("%%%%%%%%%%%%%");
	
		for(int i = 0; i<4;i++)
			printf("%d ",SecondPopulation[iPop].BreakNum[i]);
	
		printf("%%%%%%%%%%%%%");

		for(int i = 0; i<3;i++)
			printf("%f ",SecondPopulation[iPop].Gap[i]);
		
		printf("\n");
	}
	*/
}
