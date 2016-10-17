// KnapsackProblem.cpp: implementation of the CLayoutProblem class.
//
//////////////////////////////////////////////////////////////////////

#include "LayoutProblem.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLayoutProblem::CLayoutProblem()
{

}

CLayoutProblem::~CLayoutProblem()
{

}

void CLayoutProblem::Initialize(int room,int a,int b)//
{
	NumOfRoom = room;
	//NumOfFs = f;
	NumOfA = a;
	NumOfB = b;
	WidOfRoom = vector<double>(room, 0.0);
	LimOfRow = vector<double>(3, 0.0);
	LimOfSecRow = vector<double>(3, 0.0);
	vector< vector<int> > f_mat(room, vector<int>(room, 0));
	vector< vector<int> > a_mat(room, vector<int>(room, 0));

	flow = f_mat;
	adj = a_mat;
}

bool CLayoutProblem::LoadInstance(char *strFileName)
{

	int i=0,j=0,k=0;
	int tmpi;
	float tmpf;

	fstream Stream (strFileName, ios::in);
	char s[200];
	
	//房间宽
	Stream >> s;
	if(strcmp(s,"width") != 0){
		Stream.close();
		return false;
	}
	for(i=0; i < NumOfRoom; i++){
		Stream >> tmpf;
		WidOfRoom[i] = tmpf;
	}

	//流量
	Stream >> s;
	if(strcmp(s,"flow") != 0){
		Stream.close();
		return false;
	}
	for(i=0; i < NumOfRoom; i++){
		for(j=0 ; j < NumOfRoom ; j++){
			
			Stream >> tmpi;
			flow[i][j] = tmpi;
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
		LimOfRow[i] = tmpf;
	}

	//二行宽度限制
	Stream >> s;
	if(strcmp(s,"limit_wid") != 0){
		Stream.close();
		return false;
	}
	for(i=0; i < 3; i++){
		Stream >> tmpf;
		LimOfSecRow[i] = tmpf;
	}

	//走廊宽
	Stream >> s;
	if(strcmp(s,"corridor") != 0){
		Stream.close();
		return false;
	}
	Stream >> WidOfCorridor;

	//房间高度
	Stream >> s;
	if(strcmp(s,"height") != 0){
		Stream.close();
		return false;
	}
	Stream >> WidOfH;
	
	//邻接矩阵
	Stream >> s;
	if(strcmp(s,"adj") != 0){
		Stream.close();
		return false;
	}
	for(i=0; i < NumOfRoom; i++){
		for(j=0 ; j < NumOfRoom ; j++){
			
			Stream >> tmpi;
			adj[i][j] = tmpi;
		}
	}


	Flow = flow;
	Adj = adj;

	return true;
}

void CLayoutProblem::Show()
{
	for(int i=0; i < NumOfRoom; i++){
		printf("%f\n", WidOfRoom[i]);
	}

    for(int iItem=0; iItem<NumOfRoom; iItem++)
	{
		printf("\n");
		for(int iKnap=0; iKnap<NumOfRoom; iKnap++)
		{
		    printf(" %d", flow[iKnap][iItem]);
		}
		
	}
	printf("\n");
	//行宽限制
	printf("lim of row");
	for(int i=0; i < 3; i++){
		printf("%f ",LimOfRow[i]);
	}
	printf("\n");
	//二行宽度限制
	for(int i=0; i < 3; i++){
		printf("%f ",LimOfSecRow[i]); ;
	}
	printf("\n");
	//走廊宽
	printf("%f \n",WidOfCorridor);
	//房间高度
	printf("%f \n",WidOfH);
	for(int iItem=0; iItem<NumOfRoom; iItem++)
	{
		printf("\n");
		for(int iKnap=0; iKnap<NumOfRoom; iKnap++)
		{
		    printf(" %d", adj[iKnap][iItem]);
		}
		
	}

}

