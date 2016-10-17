#include "Global.h"
#include "LayoutProblem.h"
#include "MOEAD-TS.h"
#include "time.h"
#include "stdio.h"
#include "stdlib.h"
#include "iostream"
using namespace std;


int main()
{
	srand(time(NULL));

	int  MaxNumberOfRuns = 1;
	double saveTime[5];

    ifstream indata; // indata is like cin

	// read test instances from a txt file
    indata.open("TestInstances.txt"); // opens the file
    if(!indata) 
	{
        cerr << "Error: file could not be opened" << endl;
        exit(1);
	}


	char temp[1024];
	//int  pop;
	while(!indata.eof())
	{
	    
		time_t  begin,end;
		begin = clock();

	    indata>>temp;
		indata>>NumOfRoom;
	    indata>>temp;
		indata>>NumOfA;
		indata>>temp;
		indata>>NumOfB;
	    indata>>temp;
		indata>>pop;
		indata>>temp;
		indata>>Instance;
		indata>>temp;
		indata>>example;
		
		
		CLayoutProblem layout;
		char filename[1024];
		sprintf(filename,"data/example%d.%d_%d_%d", Instance, NumOfRoom, NumOfA, NumOfB);
		printf("%s",filename);
	
		layout.Initialize(NumOfRoom, NumOfA,NumOfB);

		layout.LoadInstance(filename);

		//layout.Show();
		for(int run = 1; run <= MaxNumberOfRuns; run++)
		{
			run_id = run;		
			sprintf(strFitnessType, "_TCHEBYCHEFF");	

			CMOEAD MOEAD;

			MOEAD.Run(500*pop);

		}
		end = clock();
		double current = (double)(end - begin)/CLOCKS_PER_SEC;
		printf("Total Time:%f sec\n", current);
		printf("----------------------------\n");

	}

	indata.close();
}
