//g++ -g -DDEBUG -I ../DATASTRUCTURES/HEAP -I ../DATASTRUCTURES/ -I . -c main.cpp; g++ -o spc main.o general_includes.o permutation.o
//g++ -pg -O3 -DNDEBUG -I ../DATASTRUCTURES/HEAP -I ../DATASTRUCTURES/ -I . -c main.cpp; g++ -pg -o spc main.o general_includes.o permutation.o
//g++ -O3 -DNDEBUG -I ../DATASTRUCTURES/HEAP -I ../DATASTRUCTURES/ -I . -c main.cpp; g++ -o spc main.o general_includes.o permutation.o
// ./scp input/rail1.txt 1 1 asd.txt 10

#include <iostream>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

#include <scp.h>
#include <general_includes.h>

using namespace std;

///PARAMETERS
double numTrial; //Default 10
double time_limit; //Default 10
bool unicost; //Default unicost
int seed; //Default 0

///PROBLEM DATA
int m; //#rows
int n; //#columns
float* c;//cost for each set
int* setCard;//cardinality for each set
int** set;//list of elements for each set

///STATS
float totalTime=0;
int totalSum=0;
float totalTimeOfBest = 0.0;

float stdDevTime =0.0;
float stdDevSum =0.0;

float currTime=0;
int currSum=0;
float t = 0;

int main(int argc, const char** argv)
{
	if(argc < 3)
	{
		cerr<<"Usage: "<<argv[0]<<" <Input_File>  <Number_of_Runs> <Search_Time_Limit>"<<endl;
		exit(0);
	}
	
	numTrial = atoi(argv[2]);
	time_limit = atof(argv[3]);
	
	int i,j,k;
	
	string filename = argv[1];
	ifstream read;
	read.open(filename.c_str());
	if(read.is_open())
	{	
		/*The format of these test problems is:
		number of rows (m), number of columns (n)
		the cost of each column c(j),j=1,...,n
		for each row i (i=1,...,m): the number of columns which cover
		row i followed by a list of the columns which cover row i
		*/
	
		//Read size
		read >> m;
		read >> n;
	
		// don't read costs, assume it is unicost
		c=new float[n];
		  for(i=0;i<n;i++)
		      c[i] = 1;

		setCard=new int[n];
		for(i=0;i<n;i++)
			setCard[i] = 0;

		signed char** data=new signed char*[n];
		for (i=0; i<n; i++)
			data[i]=new signed char[m];
	
		int count=0;
		int colIndex=0;
		for(i=0;i<m;i++)
		{
			read>>count;
			for(j=0;j<count;j++)
			{
				read >> colIndex;
				data[colIndex-1][i] = 1; //input starts from 1..!!!
				setCard[colIndex-1]++;
			}
		}
	
		set=new int*[n];
		for (i=0; i<n; i++)
			set[i]=new int[setCard[i]];
	
		int elt =0;
		for (i=0; i<n; i++)
		{
			elt=0;
			for(j=0;j<m;j++)
			{
				if(data[i][j] == 1)
				{
					set[i][elt] = j;
					elt++;
				}
			}
		}
		
		for (int j=0; j<n; j++)
			delete [] data[j];
		delete [] data;
		
		vector<double> timeOfBestRecord;
		timeOfBestRecord.resize(numTrial);
		vector<double> solutionRecord;
		solutionRecord.resize(numTrial);

		for (i=0; i<numTrial; i++)
		{
			t=now();
			srand(i);
			SCP scp(n,m,c,set,setCard);
			solutionRecord[i] = scp.dialektic(timeOfBestRecord[i], time_limit);
			currTime = now()-t;
//  			cout << " time: " << currTime << endl;
			
			totalTimeOfBest += timeOfBestRecord[i];
			totalSum += solutionRecord[i];
			totalTime += currTime;
		}
		
		sort(timeOfBestRecord.begin(),timeOfBestRecord.end());
		sort(solutionRecord.begin(),solutionRecord.end());
		
		double minBestTime = timeOfBestRecord[0];
		double maxBestTime = timeOfBestRecord[numTrial-1];
		double minSum = solutionRecord[0];
		double maxSum = solutionRecord[numTrial-1];
			
		//Standard Deviation
		double totalDevTime = 0.0;
		double totalDevSum = 0.0;
		for (i=0; i<numTrial; i++)
		{
			timeOfBestRecord[i] -= totalTimeOfBest/numTrial;
			timeOfBestRecord[i] = timeOfBestRecord[i]*timeOfBestRecord[i];
			totalDevTime += timeOfBestRecord[i];
			
			solutionRecord[i] -= totalSum/numTrial;
			solutionRecord[i] = solutionRecord[i]*solutionRecord[i];
			totalDevSum += solutionRecord[i];
		}
		
		cout << "Input: " << filename << endl;
	          cout << "Total runtime: " << totalTime <<endl; //Total run time of N runs
		cout << "Avg. runtime: " << totalTime/numTrial<<endl; //Runtime per instance - Cutoff
// 		cout << "Max runtime: " << maxBestTime <<endl;
// 		cout << "Min runtime: " << minBestTime <<endl;
		cout << "Avg. solution quality: " << totalSum/numTrial <<endl; //Average solution quality
		cout << "StDev solution quality: " << sqrt(totalDevSum/numTrial) <<endl;  //Std Dev in solution quality
		cout << "Best solution quality: " << minSum <<endl; //Best solution quality
		cout << "Worst solution quality: " << maxSum <<endl; //Best solution quality
	}
	else{cout<<"Error: can not open input file "<<argv[1]<<endl;}
	
	for (int j=0; j<n; j++)
		delete [] set[j];
	delete [] set;
	delete [] setCard;
	delete [] c;
	
	return 0;
}
