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

#include "mrlp.h"

typedef IloArray<IloNumVarArray> IloNumVarArray2;
typedef IloArray<IloBoolVarArray> IloBoolVarArray2;
typedef IloArray<IloIntVarArray> IloIntVarArray2;
typedef IloArray<IloArray<IloBoolVarArray>> IloBoolVarArray3;


ILOSTLBEGIN

const IloInt nbR=3;
IloInt nbN=8;	//行数 房间数
IloInt na,nb;		//2行左右房间数
IloNumArray nd,nw;			//三行走廊限制 房间宽度
//IloArray<IloNumArray> nf ;	//流量
//IloArray<IloNumArray> nadj ;	//流量
//const IloNum dl=3,dm=2.5,dr=3;		//二行左中右宽度限制
IloNumArray d2r;
IloNum nc=0,nh=0;		//走廊宽 房间高度
const IloNum nbM=1000;	//走廊宽 足够大的数
int example,Instance;
vector< vector<int> > nf;
vector< vector<int> > nadj;


//IloArray<IloNumArray> f;
bool LoadInstance(char *strFileName){
	
	int i=0,j=0,k=0;
	int tmpi;
	float tmpf;

	fstream Stream (strFileName, ios::in);
	char s[200];
	
	//房间宽
	Stream >> s;
	if(strcmp(s,"width") != 0){
		Stream.close();
		printf("false");
		return false;
	}
	for(i=0; i < nbN; i++){
		Stream >> tmpf;
		nw[i] = tmpf;
	}

	//流量
	Stream >> s;
	if(strcmp(s,"flow") != 0){
		Stream.close();
		return false;
	}
	for(i=0; i < nbN; i++){
		for(j=0 ; j < nbN ; j++){
			
			Stream >> tmpi;
			nf[i][j] = tmpi;
			//printf("%d %d",tmpi,nf[i][j]);
		}		
	}

	//行宽限制
	Stream >> s;
	if(strcmp(s,"limit_row") != 0){
		Stream.close();
		return false;
	}
	for(i=0; i < 3; i++){
		Stream >> tmpf;
		nd[i] = tmpf;
	}

	//二行宽度限制
	Stream >> s;
	if(strcmp(s,"limit_wid") != 0){
		Stream.close();
		return false;
	}
	for(i=0; i < 3; i++){
		Stream >> tmpf;
		d2r[i] = tmpf;
	}

	//走廊宽
	Stream >> s;
	if(strcmp(s,"corridor") != 0){
		Stream.close();
		return false;
	}

	Stream >> tmpf;
	nc = tmpf;

	//房间高度
	Stream >> s;
	if(strcmp(s,"height") != 0){
		Stream.close();
		return false;
	}
	Stream >> tmpf;
	nh = tmpf;
	
	//邻接矩阵
	Stream >> s;
	
	if(strcmp(s,"adj") != 0){
		Stream.close();
		return false;
	}
	for(i=0; i < nbN; i++){
		for(j=0 ; j < nbN ; j++){
			
			Stream >> tmpi;
			nadj[i][j] = tmpi;
		}
	}
	
	return true;
}

void show(){
	printf("width of room\n");
	for(int i = 0;i < nbN; i++ )
		printf("%.2f ",nw[i]);
	printf("\nwidth limit in the middle row\n");
	printf("%f %f %f\n",d2r[0],d2r[1],d2r[2]);
	printf("corridor height\n%f %f \n",nc,nh);
	printf("row limit\n%f %f %f\nflow \n",nd[0],nd[1],nd[2]);
	for(int i=0;i<nbN;i++){
		for(int j=0;j<nbN;j++)
			printf("%d ",nf[i][j]);
		printf("\n");
	}
	printf("adj\n");
	for(int i=0;i<nbN;i++){
		for(int j=0;j<nbN;j++)
			printf("%d ",nadj[i][j]);
		printf("\n");
	}
	

}

void define_data(IloEnv env,char *filename) {
	
	nd = IloNumArray(env, nbR);

	nw = IloNumArray(env, nbN);
	vector< vector<int> > f_mat(nbN, vector<int>(nbN, 0));
	vector< vector<int> > a_mat(nbN, vector<int>(nbN, 0));
	nf = f_mat;
	nadj = a_mat;
/*	nf = IloArray<IloNumArray>(env,nbN);
	nadj = IloArray<IloNumArray>(env,nbN);
	
	for(int i=0;i<nbN;i++)
		nf[i] = IloNumArray(env,nbN);
	for(int i=0;i<nbN;i++)
		nadj[i] = IloNumArray(env,nbN);
*/	
	
	d2r = IloNumArray(env, 3);
	LoadInstance(filename);
	//show();
}

bool main_cplex(IloEnv env,float a);

int main(int, char**argv) {
   ifstream indata;
   indata.open("TestInstances.txt");
   if(!indata) 
	{
        cerr << "Error: file could not be opened" << endl;
        exit(1);
	}

   char temp[1024];
   while(! indata.eof()){
	    IloEnv env;
		time_t  begin,end;
		begin = clock();

	    indata>>temp;
		indata>>nbN;
	    indata>>temp;
		indata>>na;
		indata>>temp;
		indata>>nb;
		indata>>temp;
		indata>>Instance;
		indata>>temp;
		indata>>example;
		char filename[1024];
		sprintf(filename,"data/example%d.%d_%d_%d", Instance, nbN, na, nb);
		printf("%s\n",filename);
		define_data(env,filename);
		for(int i=0;i>=0;i--){
			float a = i*0.1;
			main_cplex(env,a);
			
		}
		//IloInt aaa;cin>>aaa;
		env.end();
   }
   return true;
}
   

  // main_cplex();
      



bool main_cplex(IloEnv env,float a){
	
	
   
	try {
      IloInt i,j,r,n,i1;
	  IloModel model(env);
	  IloNumVarArray2 p;	//位置表示
	  IloIntVarArray2 x;	//是否在r行
	  IloBoolVarArray2 q;	//是否在2行
	  IloBoolVarArray3 z;	//i与j是否在同行且i在j左边
	  p = IloNumVarArray2(env,nbN);
	  x = IloIntVarArray2(env,nbN);
	  q = IloBoolVarArray2(env,nbN);
	  z = IloBoolVarArray3(env,nbN);

	  for (i=0;i<nbN;i++){
		  p[i] = IloNumVarArray(env,nbR,0,100);
		  x[i] = IloIntVarArray(env,nbR,0,1);
		  q[i] = IloBoolVarArray(env,na+nb);
		  z[i] = IloBoolVarArray2(env,nbN);
		  for(j=0;j<nbN;j++){
			  z[i][j]=IloBoolVarArray(env,nbR);
		  }
	  }
	
	  //constraints 1
	  for(i=0;i<nbN;i++){
		  model.add(x[i][0]+x[i][1]+x[i][2]==1);
	  }
	  
	  //constraints 2
	  IloIntExpr sum2(env);
	  for(i=0;i<nbN;i++){
		  sum2+=x[i][1];
	  }
	  model.add(sum2==na+nb);
	  sum2.end();
	  
	  //constraints 3+4+5
	  for(i=0;i<nbN;i++){
		  for(j=0;j<nbN;j++){
			  if(i==j) continue;
			  for(r=0;r<nbR;r++){
				  model.add(z[i][j][r]+z[j][i][r] <= IloMin(x[i][r],x[j][r]));
				  model.add(z[i][j][r]+z[j][i][r]+1 >= x[i][r]+x[j][r]);
				  model.add(nbM*(1-z[i][j][r])+p[j][r]-p[i][r]>=(nw[i]+nw[j])/2);
	//			  model.add(nbM*(1-z[j][i][r])+p[i][r]-p[j][r]>=(nw[i]+nw[j])/2);

			  }
		  }
	  }
	  
	  //constraints 7+11
	  for(i=0;i<nbN;i++){
		  for(r=0;r<nbR;r++){
			  IloNumExpr sum4c(env);
			  sum4c=nd[r]-nw[i]*x[i][r]/2-d2r[2]*x[i][1];
			  model.add(p[i][r]<=IloMin(nbM*x[i][r],sum4c));
			  sum4c.end();
		  }
	  }
	  
	  //constraints 8 try
	  for(i=0;i<nbN;i++){
		  IloIntExpr sum6a(env);
		  for(n=0;n<na+nb;n++){
			  sum6a+=q[i][n];
		  }
		  model.add(sum6a == x[i][1]);
		  sum6a.end();
	  }
 	  //constraints 9
	  for(n=0;n<na+nb;n++){
		  IloIntExpr sum10(env);
		  for(i=0;i<nbN;i++){
			  sum10+=q[i][n];
		  }
		  model.add(sum10==1);
		  sum10.end();

	  }

	  //constraints 10
	  for(i=0;i<nbN;i++){
		  for(r=0;r<nbR;r++){
			  if(r==1) 
				  model.add(p[i][r]>=nw[i]*x[i][r]/2+d2r[0]*x[i][1]);
			  else  
				  model.add(p[i][r]>=nw[i]*x[i][r]/2);
		  }
	  }
	  
	  //constraints 12 try
	  for(i=0;i<nbN;i++){
		  for(r=0;r<nbR;r++){
			  if(r==1)continue;
			  else{
				  IloNumExpr sum7b(env);
				  for(j=0;j<nbN;j++){
					  if(j==i)continue;
					  sum7b += z[j][i][r]*nw[j];
				  }
				  model.add(p[i][r] == sum7b+(nw[i]*x[i][r])/2);
				  sum7b.end();
			  }
		  }
	  }
	 
	  //constraints 13 14 15 trying
	   for(i=0;i<nbN;i++){
		   for(j=0;j<nbN;j++){  
			   if(i==j) continue;
			   for(i1=0;i1<na+nb-1;i1++){
				   if(i1==na-1){
					   model.add(nbM*(2-q[i][na-1]-q[j][na])+p[j][1]-p[i][1]>=0.5*(nw[i]+nw[j])+d2r[1]);
					   continue;
				   }
				   model.add(nbM*(2-q[i][i1]-q[j][i1+1])+p[j][1]-p[i][1]>=0.5*(nw[i]+nw[j]));
				   model.add(nbM*(q[i][i1]+q[j][i1+1]-2)+p[j][1]-p[i][1]<=0.5*(nw[i]+nw[j]));
			   }
		   }  
	   }

	   
	  
	  IloNumExpr fun(env);
	  for(i=0;i<nbN-1;i++){
		  for(j=i+1;j<nbN;j++){
			  if(i==j) continue;
			  IloNumExpr sum1(env),sum2(env),sumb(env);
			  for(r=0;r<nbR;r++){
				  sum1+=p[i][r];
				  sum2+=p[j][r];
				  if(r==1) continue;
				  else{
					  sumb+=IloAbs(x[i][r]-x[j][r]);
				  }
			  }
			  fun+=(a*nf[i][j]+(1.0-a)*nadj[i][j])*(IloAbs(sum1-sum2)+(nc+nh)*sumb);
			  sum1.end();
			  sum2.end();
			  sumb.end();
		  }
	  }
	  
	

	  model.add(IloMinimize(env,fun));
	  fun.end();

	  IloCplex cplex(env);
	  
	  cplex.setParam(IloCplex::NodeFileInd,2);
	  cplex.setParam(IloCplex::MemoryEmphasis,true);
	  cplex.setParam(IloCplex::WorkMem,1024);
	  cplex.setParam(IloCplex::TreLim,1024);
	  cplex.setParam(IloCplex::Threads,1) ;  
	  cplex.setParam(IloCplex::VarSel,3);
		
	  cplex.setParam (IloCplex::WorkMem ,2000);
	  cplex.setParam(IloCplex::NodeFileInd ,3);
	  //cplex.setParam (IloCplex::Threads ,1);
	  
	  cplex.setParam (IloCplex::TiLim,20);
	  cplex.extract(model);
	  IloTimer timer(env);
	  timer.start();
	  cplex.solve();
	  timer.stop();



	  if(cplex.getStatus() == IloAlgorithm::Infeasible)
         env.out() << "No Solution" << endl;
	  cout<<endl;
	 
	  char saveFilename[1024];
	  sprintf(saveFilename,"POF/Room%d_instance_%d_parameter%.1f_%.1f.data", nbN, Instance, a, 1-a);
	  std::fstream fout;
	  fout.open(saveFilename,std::ios::out);
	  fout.precision(12);
	  fout<<"--------------------------------\n";
	  
	 
	  //output
      //cplex.solve();	  
	  
	  fout<<"time is:"<<timer.getTime()<<endl;

	  fout << "----------------------------------------" << endl;
	  fout<<"optimal value:"<<cplex.getObjValue()<<endl;

	  fout << "----------------------------------------" << endl;
	  fout<<"room location"<<endl;
	  
	  for(r=0;r<nbR;r++){
		  for(i=0;i<nbN;i++){
			  fout<<cplex.getValue(p[i][r])<<"  ";
			  
		  }
		  fout<<endl;
	  }
	  fout<<" 以上——————————————————以下zijr"<<endl;
	  for(r=0;r<nbR;r++){
		  for(i=0;i<nbN;i++){ 
			  for(j=0;j<nbN;j++){
				  if(i==j) {fout<<"0  ";continue;}
				  fout<<cplex.getValue(z[i][j][r])<<"  ";
			  }
			  fout<<endl;
		  }
		  fout<<endl;
	  }

	  fout <<"----------------------------------------以下qin" << endl;
		  
	  for(j=0;j<nbN;j++){
		  for(n=0;n<4;n++){
				  fout<<cplex.getValue(q[j][n])<<"  ";
		  }
		  fout<<endl;
	  }
	  fout <<"----------------------------------------以下xir" << endl;
	  for(r=0;r<nbR;r++){
		  for(i=0;i<nbN;i++){
			  fout<<cplex.getValue(x[i][r])<<"  ";
			  
		  }
		  fout<<endl;
	  }
	  printf("最优值为%f\n",cplex.getObjValue());
	  fout<<"best answer"<<cplex.getObjValue()<<endl;
	  
	  double fun_f=0.0,fun_a=0.0;
	  for(i=0;i<nbN-1;i++){
		  for(j=i+1;j<nbN;j++){
			  if(i==j) continue;
			  double sum1=0,sum2=0;
			  float temp=0.0;
			  int sumb=0;
			  for(r=0;r<nbR;r++){
				  temp=double(cplex.getValue(p[i][r]));
				  sum1 += temp;
				  temp=double(cplex.getValue(p[j][r]));
				  sum2 += temp;
				  if(r==1) continue;
				  else{
					  sumb += abs(int(cplex.getValue(x[i][r]))-int(cplex.getValue(x[j][r])));
				  }
			  }
			 // printf("%f %f %d\n",sum1,sum2,sumb);
			  fun_f+=nf[i][j]*(abs(sum1-sum2)+(nc+nh)*sumb);
			  fun_a+=nadj[i][j]*(abs(sum1-sum2)+(nc+nh)*sumb);
		  }
	  }
	  fout<<"flow answer:"<<fun_f<<endl;
	  fout<<"adj answer:"<<fun_a<<endl;
	  fout.close();
/*	  if(cplex.solve())
      cout << " Minimize cost = " << cplex.getObjValue() << endl<<endl;
	  }*/
	  return true;
	  }
	  catch (IloException& ex) {
		  cerr << "Error: " << ex << endl;
		  cerr<<ex.getMessage()<<endl;
		  return false;
	  }
	  catch (...) {
		  cerr << "Error" << endl;
		  return false;
	  }
	  
	  return 0;
}