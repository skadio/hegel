
#ifndef SCP_H
#define SCP_H

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <general_includes.h>
#include <PERMUTATION/binom.h>
#include <HEAP/heap.h>
#include <SET/set.h>

using namespace std;
const int numberOfHeuristics=23;
const int rankCard=5;
#define INF 9999999

float sqr(float a){return a*a;}

int chooseDist(float* dis, int n)
{
	if (n<=0) return -1;
	long long* D = new long long[n];
	float acc=0.;
	for (int i=0; i<n; i++)
	{
		acc+=dis[i];
		D[i]=(int) minimum(acc*1000000.,1000000.);
	}
	D[n-1]=1000000;
	long long r=rand()%1000000;
	int x;
	for (x=0;x<n; x++)
		if (r<D[x]) break;
	assert(x<n);
	delete [] D;
	return x;
}

class SCP
{
	//instance data
	int n,m;         //numbers sets and items
	float* c;        //cost vector
	int** set;       //given sets
	int* setCard;    //cardinality of each set

	int** appears;   //for each item a list of sets the item appears in
	int* appearCard; //in how many sets does each item appear

	float maxCost;
	float minCost;

	//solution data
	Set** covered;    // for each item, a list of all sets currently covering it
	int uncovered;    // how many items are currently uncovered
	float cost;       // cost of all selected sets
	Set selection;    // which sets are currently selected
	Set complement;   // which sets are currently not selected
	int* newCover;    // for each set, how many new items would it cover if added
	int* disCover;    // for each set, how many items rely on it for being covered
	Set best;
	float bestVal;
	
	float EverBest;

	void alloc(void)
	{
		covered=new Set*[m];
		disCover=new int[n];
		newCover=new int[n];
		appears=new int*[m];
		appearCard=new int[m];
		for (int i=0; i<m; i++)
		{
			appears[i]=new int[n];
			covered[i]=new Set(n);
		}
	}

	void print()
	{
		cout<<"Cost: "<<cost<<endl;
		cout<<"Uncovered: "<<uncovered<<endl;
		cout<<"Best(val: "<<bestVal<<" size: "<<best.card<<")"<<flush;
		for(int i=0;i<best.card;i++)
			cout<<best.list[i]<<" ";
		cout<<endl;
		cout<<"Selected("<<selection.card<<") "<<flush;
		for(int i=0;i<selection.card;i++)
			cout<<selection.list[i]<<" ";
		cout<<endl;
		cout<<"Complement("<<complement.card<<") "<<flush;
		for(int i=0;i<complement.card;i++)
			cout<<complement.list[i]<<" ";
		cout<<endl;
		cout<<"Item covered by: "<<flush;
		for(int i=0;i<m;i++)
		{
			cout<<"Item"<<i<<"("<<flush;
			for(int j=0;j<covered[i]->card;j++)
			{
				cout<<covered[i]->list[j]<<" ";
			}
			cout<<")"<<endl;
		}
		cout<<"NewCover: "<<flush;
		for(int i=0;i<n;i++)
			cout<<newCover[i]<<" ";
		cout<<endl;
		cout<<"DisCover: "<<flush;
		for(int i=0;i<n;i++)
			cout<<disCover[i]<<" ";
		cout<<endl;
				
		cout<<endl;
	}
	
	void free(void)
	{
		delete [] disCover;
		delete [] newCover;
		delete [] appearCard;
		for (int i=0; i<m; i++)
		{
			delete [] appears[i];
			delete covered[i];
		}
		delete [] covered;
		delete [] appears;
	}

	void reset(void)
	{
		uncovered=m;
		cost=0.;
		for (int i=0; i<m; i++)
			covered[i]->remAll();
		for (int j=0; j<n; j++)
		{
			newCover[j]=setCard[j];
			disCover[j]=0;
		}
		for (int l=0; l<selection.card; l++)
		{
			bool r=complement.add(selection.list[l]);
			assert(r);
		}
		selection.remAll();
	}

	void init(void)
	{
		for (int i=0; i<m; i++)
			appearCard[i]=0;

		maxCost=-1;
		minCost=INF;
		for (int j=0; j<n; j++)
		{
			bool r=complement.add(j);
			assert(r);
			assert(c[j]>0);
			maxCost=maximum(maxCost,c[j]);
			minCost=minimum(minCost,c[j]);
			for (int k=0; k<setCard[j]; k++)
			{
				int i=set[j][k];
				appears[i][appearCard[i]++]=j;
			}
		}
		reset();
	}


	int h0(void)
	//uniform random
	{
		int numberOfCandidates=0;
		int* candidates=new int[n];
		int b=-1;
		for (int j=0; j<n; j++)
		{
			if (newCover[j]<=0) continue;
			candidates[numberOfCandidates++]=j;
		}
		if (numberOfCandidates==0) 
		{
			delete [] candidates;
			return -1;
		}
		b=candidates[rand()%numberOfCandidates];
		delete [] candidates;
		return b;
	}

	int h1(void)
  	//min cost
	{
		float minScore=INF;
		int b=-1;
		for (int j=0; j<n; j++)
		{
			if (newCover[j]<=0) continue;
			float s=c[j];
			if (minScore>s)
			{
				minScore=s;
				b=j;
			}
		}
		return b;
	}


	int h8(void)
 	 //min cost random
	{
		int cand=0;
		float* dis=new float[n];
		int* col=new int[n];
		float sum=0;
		for (int j=0; j<n; j++)
		{
			if (newCover[j]<=0) continue;
			dis[cand]=maxCost+1-c[j];
			col[cand]=j;
			sum+=dis[cand++];
		}
		for (int k=0; k<cand; k++)
			dis[k]/=sum;
		int b= chooseDist(dis,cand);
		if (b>=0) b=col[b];
		delete [] dis;
		delete [] col;
		return b;
	}

	int h16(void)
  	//min cost rank
	{
		HHeap<int,float> heap(n);
		for (int j=0; j<n; j++)
		{
			if (newCover[j]<=0) continue;
			heap.ins(j,c[j]);
		}
		if (heap.getSize()==0) return -1;
		int mi=minimum((long)rankCard,heap.getSize());
		int r=rand()%mi;
		for (int k=1; k<r; k++)
			heap.del();
		return heap.min();
	}

	int h9(void)
  	//min cost random
	{
		int cand=0;
		float* dis=new float[n];
		int* col=new int[n];
		float sum=0;
		for (int j=0; j<n; j++)
		{
			if (newCover[j]<=0) continue;
			dis[cand]=1/c[j];
			col[cand]=j;
			sum+=dis[cand++];
		}
		for (int k=0; k<cand; k++)
			dis[k]/=sum;
		int b= chooseDist(dis,cand);
		if (b>=0) b=col[b];
		delete [] dis;
		delete [] col;
		return b;
	}

	int h2(void)
  	//max new cover
	{
		float maxScore=-1;
		int b=-1;
		for (int j=0; j<n; j++)
		{
			if (newCover[j]<=0) continue;
			float s=newCover[j];
			if (maxScore<s)
			{
				maxScore=s;
				b=j;
			}
		}
		return b;
	}

	int h10(void)
 	//max new cover random
	{
		int cand=0;
		float* dis=new float[n];
		int* col=new int[n];
		float sum=0;
		for (int j=0; j<n; j++)
		{
			if (newCover[j]<=0) continue;
			dis[cand]=newCover[j];
			col[cand]=j;
			sum+=dis[cand++];
		}
		for (int k=0; k<cand; k++)
			dis[k]/=sum;
		int b= chooseDist(dis,cand);
		if (b>=0) b=col[b];
		delete [] dis;
		delete [] col;
		return b;
	}

	int h17(void)
  	//max new cover rank
	{
		HHeap<int,float> heap(n);
		for (int j=0; j<n; j++)
		{
			if (newCover[j]<=0) continue;
			heap.ins(j,-newCover[j]);
		}
		if (heap.getSize()==0) return -1;
		int mi=minimum((long)rankCard,heap.getSize());
		int r=rand()%mi;
		for (int k=1; k<r; k++)
			heap.del();
		return heap.min();
	}


	int h11(void)
  	//min cost over cover random
	{
		int cand=0;
		float* dis=new float[n];
		int* col=new int[n];
		float sum=0;
		for (int j=0; j<n; j++)
		{
			if (newCover[j]<=0) continue;
			dis[cand]=newCover[j]/c[j];
			col[cand]=j;
			sum+=dis[cand++];
		}
		for (int k=0; k<cand; k++)
			dis[k]/=sum;
		int b= chooseDist(dis,cand);
		if (b>=0) b=col[b];
		delete [] dis;
		delete [] col;
		return b;
	}

	int h18(void)
  	//min cost over cover rank
	{
		HHeap<int,float> heap(n);
		for (int j=0; j<n; j++)
		{
			if (newCover[j]<=0) continue;
			heap.ins(j,c[j]/newCover[j]);
		}
		if (heap.getSize()==0) return -1;
		int mi=minimum((long)rankCard,heap.getSize());
		int r=rand()%mi;
		for (int k=1; k<r; k++)
			heap.del();
		return heap.min();
	}

	int h4(void)
  	//c/log new cover
	{
		float minScore=INF;
		int b=-1;
		for (int j=0; j<n; j++)
		{
			if (newCover[j]<=0) continue;
			float s=c[j]/log(newCover[j]+1);
			if (minScore>s)
			{
				minScore=s;
				b=j;
			}
		}
		return b;
	}

	int h12(void)
  	//c/log random
	{
		int cand=0;
		float* dis=new float[n];
		int* col=new int[n];
		float sum=0;
		for (int j=0; j<n; j++)
		{
			if (newCover[j]<=0) continue;
			dis[cand]=log(newCover[j])/c[j];
			col[cand]=j;
			sum+=dis[cand++];
		}
		for (int k=0; k<cand; k++)
			dis[k]/=sum;
		int b= chooseDist(dis,cand);
		if (b>=0) b=col[b];
		delete [] dis;
		delete [] col;
		return b;
	}

	int h19(void)
  	//min cost over log rank
	{
		HHeap<int,float> heap(n);
		for (int j=0; j<n; j++)
		{
			if (newCover[j]<=0) continue;
			heap.ins(j,c[j]/log(newCover[j]));
		}
		if (heap.getSize()==0) return -1;
		int mi=minimum((long)rankCard,heap.getSize());
		int r=rand()%mi;
		for (int k=1; k<r; k++)
			heap.del();
		return heap.min();
	}

	int h5(void)
 	//c/new cover * log new cover
	{
		float minScore=INF;
		int b=-1;
		for (int j=0; j<n; j++)
		{
			if (newCover[j]<=0) continue;
			float s=c[j]/(newCover[j]*log(newCover[j]+1));
			if (minScore>s)
			{
				minScore=s;
				b=j;
			}
		}
		return b;
	}

	int h13(void)
  	//c/k log k random
	{
		int cand=0;
		float* dis=new float[n];
		int* col=new int[n];
		float sum=0;
		for (int j=0; j<n; j++)
		{
			if (newCover[j]<=0) continue;
			dis[cand]=newCover[j]*log(newCover[j])/c[j];
			col[cand]=j;
			sum+=dis[cand++];
		}
		for (int k=0; k<cand; k++)
			dis[k]/=sum;
		int b= chooseDist(dis,cand);
		if (b>=0) b=col[b];
		delete [] dis;
		delete [] col;
		return b;
	}

	int h20(void)
  	//min cost over k log k rank
	{
		HHeap<int,float> heap(n);
		for (int j=0; j<n; j++)
		{
			if (newCover[j]<=0) continue;
			heap.ins(j,c[j]/(newCover[j]*log(newCover[j])));
		}
		if (heap.getSize()==0) return -1;
		int mi=minimum((long)rankCard,heap.getSize());
		int r=rand()%mi;
		for (int k=1; k<r; k++)
			heap.del();
		return heap.min();
	}

	int h6(void)
  	//c/new cover^2
	{
		float minScore=INF;
		int b=-1;
		for (int j=0; j<n; j++)
		{
			if (newCover[j]<=0) continue;
			float s=c[j]/(newCover[j]*newCover[j]);
			if (minScore>s)
			{
				minScore=s;
				b=j;
			}
		}
		return b;
	}

	int h14(void)
  	//c/k2 random
	{
		int cand=0;
		float* dis=new float[n];
		int* col=new int[n];
		float sum=0;
		for (int j=0; j<n; j++)
		{
			if (newCover[j]<=0) continue;
			dis[cand]=sqr(newCover[j])/c[j];
			col[cand]=j;
			sum+=dis[cand++];
		}
		for (int k=0; k<cand; k++)
			dis[k]/=sum;
		int b= chooseDist(dis,cand);
		if (b>=0) b=col[b];
		delete [] dis;
		delete [] col;
		return b;
	}

	int h21(void)
  	//min cost over k2 rank
	{
		HHeap<int,float> heap(n);
		for (int j=0; j<n; j++)
		{
			if (newCover[j]<=0) continue;
			heap.ins(j,c[j]/sqr(newCover[j]));
		}
		if (heap.getSize()==0) return -1;
		int mi=minimum((long)rankCard,heap.getSize());
		int r=rand()%mi;
		for (int k=1; k<r; k++)
			heap.del();
		return heap.min();
	}

	int h7(void)
  	//sqrt(c)/new cover^2
	{
		float minScore=INF;
		int b=-1;
		for (int j=0; j<n; j++)
		{
			if (newCover[j]<=0) continue;
			float s=sqrt(c[j])/(newCover[j]*newCover[j]);
			if (minScore>s)
			{
				minScore=s;
				b=j;
			}
		}
		return b;
	}

	int h15(void)
  	//sqrt c/k2 random
	{
		int cand=0;
		float* dis=new float[n];
		int* col=new int[n];
		float sum=0;
		for (int j=0; j<n; j++)
		{
			if (newCover[j]<=0) continue;
			dis[cand]=sqr(newCover[j])/sqrt(c[j]);
			col[cand]=j;
			sum+=dis[cand++];
		}
		for (int k=0; k<cand; k++)
			dis[k]/=sum;
		int b= chooseDist(dis,cand);
		if (b>=0) b=col[b];
		delete [] dis;
		delete [] col;
		return b;
	}

	int h22(void)
  	//min sqrt cost over k2 rank
	{
		HHeap<int,float> heap(n);
		for (int j=0; j<n; j++)
		{
			if (newCover[j]<=0) continue;
			heap.ins(j,sqrt(c[j])/sqr(newCover[j]));
		}
		if (heap.getSize()==0) return -1;
		int mi=minimum((long)rankCard,heap.getSize());
		int r=rand()%mi;
		for (int k=1; k<r; k++)
			heap.del();
		return heap.min();
	}
	
	bool moveH3(Set& pos, Set& neg)
	{
		bool res=false;
		int b=h3(pos,neg);
		while (b!=-1)
		{

// 			cout << "suggested " << b << endl;
// 			cout << "pos -> " << pos << endl;
// 			cout << "neg -> " << neg << endl << flush;
	
			if (cost<=bestVal+0.00001 && uncovered==0)
			{
// 	    			if (cost<bestVal) cout << cost << " ";
//				cout << cost << "8888888888888888";
				res=cost<bestVal;
				best=selection;
				bestVal=cost;
			}
			if (pos.rem(b))
			{
				assert(!selection.isInSet(b));
				commit(b);
	    			//cout << "+" << flush;
				b=h3(pos,neg);
				continue;
			}
			if ((pos.card==0)&&(uncovered>0))
			{
				for (int l=0; l<neg.card; l++)
				{
					int b=neg.list[l];
					//cout << "-" << flush;
					assert(selection.isInSet(b));
					unCommit(b);
				}
				neg.remAll();
				break;
			}
			neg.rem(b);
			assert(selection.isInSet(b));
			unCommit(b);
			//cout << "-" << flush;
			b=h3(pos,neg);
		}
		//cout << endl;
		return res;
	}

	void moveTo(Set& s)
	{
		for (int i=0; i<n; i++)
		{
			if (selection.isInSet(i)==s.isInSet(i)) continue;
			if (selection.isInSet(i))
			{
				unCommit(i);
				continue;
			}
			commit(i);
		}
	}
	
	int h3(void)
  	//c/new cover
	{
		float minScore=INF;
		int b=-1;
		for (int j=0; j<n; j++)
		{
			if (newCover[j]<=0) continue;
			float s=c[j]/newCover[j];
			if (minScore>s)
			{
				minScore=s;
				b=j;
			}
		}
		return b;
	}

	int h3p(const Set& pos)
  	//c/new cover
	{
		if (pos.card==0) return -1;
		float minScore;
		float minCost=INF;
		int b=-1;
		int d=-1;
		int l;
		//Setin icinde
		for (l=0; l<pos.card; l++)
		{
			int j=pos.list[l];
			//selection a eklense faydasi olmuycak adam
			if (newCover[j]<=0) 
			{
				//minimum costluyu al
				if (minCost>c[j])
				{
					d=j;
					minCost=c[j];
				}
				continue;
			}
			//Faydasi olucak adam varsa
			minScore=c[j]/newCover[j];
			b=j;
			break;
		}
		
		assert(l<pos.card||d>=0);
		//eklense faydasi olmayan adami dondur
		if (l==pos.card) return d;
		
		//Faydasi olacak adam varsa
		for (; l<pos.card; l++)
		{
			int j=pos.list[l];
			if (newCover[j]<=0) continue;
			float s=c[j]/newCover[j];
			//daha ucuzu olabilir mi?
			if (minScore>s)
			{
				minScore=s;
				b=j;
			}
		}
		assert(b>=0);
		return b;
	}
	
	int h3n(const Set& neg)
	//dis cover over c
	{
		if (neg.card==0) return -1;
		float minScore=INF;
		float maxC;
		int b=-1;
		int d=-1;
		int l;
		for (l=0; l<neg.card; l++)
		{
			int j=neg.list[l];
			//Selectionda bu sete bagli adam varsa
			if (disCover[j]>0)
			{
				//setin costuna bak
				float s=disCover[j]/c[j];
				if (minScore>s)
				{
					//minimum al
					minScore=s;
					d=j;
				}
				continue;
			}
			//set e kimse bagli degilse direk at.
			b=j;
			maxC=c[j];
			break;
		}
		assert(l<neg.card||d>=0);
		//hic gereksiz set yoksa minimum discover/c dondur
		if (l==neg.card) return d;
		
		for (; l<neg.card; l++)
		{
			int j=neg.list[l];
			if (disCover[j]>0) continue;
			//baska gereksiz varsa daha costlu
			if (maxC<c[j])
			{
				maxC=c[j];
				b=j;
			}
		}
		assert(b>=0);
		//onu dondur
		return b;
	}
 
	int h3(const Set& pos, const Set& neg)
  	//c/new cover
	{
		if (pos.card==0)
		{
			if (neg.card==0) return -1;
			return h3n(neg);
		}
		if (neg.card==0) return h3p(pos);
		if (uncovered>0) return h3p(pos);
		int b=h3n(neg);
		if (disCover[b]==0) return b;
		return h3p(pos);
	}

	public:

		SCP(int _n, int _m, float* _c, int** _set, int* _setCard) : n(_n), m(_m), c(_c), set(_set), setCard(_setCard), selection(_n), complement(_n), best(_n)
		{
			alloc();
			init();
		}
 
		~SCP(void)
		{
			free();
		}

		//Remove redundant sets from the current selection
		void removeRedundant()
		{
			for(int i=0;i<selection.card;i++)
			{
				//redundant = no itmes depends on it
				if(disCover[selection.list[i]]==0)
				{
					unCommit(selection.list[i]);
					i=-1;
				}
			}
		}
		
		float dialektic(double &timeBest, double time_limit)
		{
			int i,j;
			
			double start = now();
			
			//compute initial cover
			float* dist=new float[numberOfHeuristics];
			for (i=0; i<numberOfHeuristics; i++)
				dist[i]=0.;
			dist[1]=0.05;
			dist[3]=0.25;
			dist[4]=0.15;
			dist[5]=0.25;
			dist[6]=0.15;
			dist[7]=0.15;
			
			//Find an initial solution according to the distribution
			float r=upperBound(dist,best,50);
			
			//Go to initial solution
			moveTo(best);
			
			//Remove redundant sets from the initial solution, if any
			removeRedundant();
			
			timeBest = now()-start;
			bestVal = cost;
			
			int noImp = 0;
			int trial = 0;
			int thesisVal = 0;
			int synthesisVal = 0;
			
			//Run for 10 seconds
			while(now() - start < time_limit)
			{
				//Trial is randomized over m, number of items
				trial = m/(rand()%3+2);
// 				trial = 100;
				
				noImp=0;

				//MOVE-1
				//Until there is no improvement
				while(noImp++<trial)
				{
					//Sets that are not in the current selection are available to add
					Set available = complement;	
					Set thesis = selection;
					thesisVal = cost;
					
					//Choose a random subset size: 1/2 or 1/3 or 1/4 of selection
					int numCandids = selection.card/(rand()%3+2);

					//Remove sets from selection greedily: the set that least items rely on it for being covered
					for(i=0;i<numCandids;i++)
					{
						//Randomize over 1st and 2nd set.
						int leastCover2 = disCover[selection.list[0]];
						int leastCover = disCover[selection.list[0]];
						int leastCoverSet = selection.list[0];
						int leastCoverSet2 = selection.list[0];
						
						//Pick top 2 sets that least items rely on it for being covered
						for (int j=1; j<selection.card; j++)
						{
							if(disCover[selection.list[j]] < leastCover)
							{
								leastCover2 = leastCover;
								leastCoverSet2 = leastCoverSet;
								
								leastCover = disCover[selection.list[j]];
								leastCoverSet = selection.list[j];
							}
						}
						
						//Remove the 1st or 2nd set that covers the least items
						if(rand()%2==0) unCommit(leastCoverSet);
						else unCommit(leastCoverSet2);
					}
					
					//Now selection is infeasible!
				
					//Select sets from "available" greedily: pick the set that covers the most items
					//Keep adding until we are feasible
					while(uncovered > 0)
					{
						//pick the set that covers the most items
						int mostCover = newCover[available.list[0]];
						int mostCoverSet = available.list[0];
						
						for (int j=1; j<available.card; j++)
						{
							if(newCover[available.list[j]] > mostCover)
							{
								mostCover = newCover[available.list[j]];
								mostCoverSet = available.list[j];
							}
						}
						
						//add the set that covers the most items
						if(mostCover > 0)
						{
							commit(mostCoverSet);
							available.rem(mostCoverSet);
						}
						else //if "available" is not enough to cover all items
						{
							//until reaching a cover
							while(uncovered>0)
							{
								//pick the set that covers the most items from any unselected set
								mostCover = -1;
								for (int j=0; j<complement.card; j++)
								{
									if(newCover[complement.list[j]] > mostCover)
									{
										mostCoverSet = complement.list[j];
									}
								}
								//add the set
								commit(mostCoverSet);
							}
						}
					}
					
					//Now we have a cover!
					
					//Remove any redundant set, if any
					removeRedundant();
					synthesisVal = cost;
					
					//If <= update the best otherwise go back to thesis
					if (thesisVal<synthesisVal)
					{
						//Go back to thesis
						moveTo(thesis);
					}
					else
					{
						if(synthesisVal<bestVal)
						{
							bestVal = synthesisVal;
							timeBest = now()-start;
							best=selection;
						}
						if(synthesisVal<thesisVal)
						{
							noImp = 0;
						}
					}
				}
				
				//Local limit exceeded...GO TO THE ANTITHESIS!
				//Sets that are not in the current selection are available to add
				Set available = complement;	
					
				//Choose a random subset size: 1/2 or 1/3 or 1/4 of selection
				int numCandids = selection.card/(rand()%3+2);

				
				//Remove sets from selection greedily: the set that least items rely on it for being covered
				for(i=0;i<numCandids;i++)
				{
						//Randomize over 1st and 2nd set.
					int leastCover2 = disCover[selection.list[0]];
					int leastCover = disCover[selection.list[0]];
					int leastCoverSet = selection.list[0];
					int leastCoverSet2 = selection.list[0];
						
						//Pick top 2 sets that least items rely on it for being covered
					for (int j=1; j<selection.card; j++)
					{
						if(disCover[selection.list[j]] < leastCover)
						{
							leastCover2 = leastCover;
							leastCoverSet2 = leastCoverSet;
								
							leastCover = disCover[selection.list[j]];
							leastCoverSet = selection.list[j];
						}
					}
						
						//Remove the 1st or 2nd set that covers the least items
					if(rand()%2==0) unCommit(leastCoverSet);
					else unCommit(leastCoverSet2);
				}
					
				//Now selection is infeasible!
				
				//Select sets from "available" greedily: pick the set that covers the most items
				//Keep adding until we are feasible
				while(uncovered > 0)
				{
					//pick the set that covers the most items
					int mostCover = newCover[available.list[0]];
					int mostCoverSet = available.list[0];
						
					for (int j=1; j<available.card; j++)
					{
						if(newCover[available.list[j]] > mostCover)
						{
							mostCover = newCover[available.list[j]];
							mostCoverSet = available.list[j];
						}
					}
						
					//add the set that covers the most items
					if(mostCover > 0)
					{
						commit(mostCoverSet);
						available.rem(mostCoverSet);
					}
					else //if "available" is not enough to cover all items
					{
						//until reaching a cover
						while(uncovered>0)
						{
							//pick the set that covers the most items from any unselected set
							mostCover = -1;
							for (int j=0; j<complement.card; j++)
							{
								if(newCover[complement.list[j]] > mostCover)
								{
									mostCoverSet = complement.list[j];
								}
							}
							//add the set
							commit(mostCoverSet);
						}
					}
				}
					
				//Now we have a cover!
				//Remove any redundant set, if any
				removeRedundant();

				if(cost<bestVal)
				{
					bestVal = cost;
					timeBest = now()-start;
					best=selection;
				}
			}
			

		        // +1 ekle columnlara 
		        cout<<"Number of selected columns: "<< best.card << endl;
		        cout<<"Selected Columns: " << flush;
		        for(int i=0;i<best.card;i++)
			   cout<<best.list[i] + 1 <<" " << flush;
		        cout<<endl;
			return bestVal;
		}
		
float hegel(void)
{
			//compute initial cover
	float* dist=new float[numberOfHeuristics];
	for (int i=0; i<numberOfHeuristics; i++)
		dist[i]=0.;
	dist[1]=0.05;
	dist[3]=0.25;
	dist[4]=0.15;
	dist[5]=0.25;
	dist[6]=0.15;
	dist[7]=0.15;
	Set bSet(n);
	bestVal=INF;

	for (int p=0; p<5; p++)
	{
		if (!p)
		{
			float r=upperBound(dist,bSet,30);
			if (r<=bestVal)
			{
				bestVal=r;
				best=bSet;
			}
		}
		else
		{
			float r=upperBound(dist,bSet,15);
			if (r<=bestVal)
			{
				bestVal=r;
				best=bSet;
			}
		}
				
		moveTo(bSet);
		cout<<"Ilk set secildi "<<bestVal<<endl;
// 				print();
				
		int noIm=0;
		while (noIm<500)
		{
			noIm++;
			//cout << bestVal << " " << flush << endl;
			//select flip variables -> so far purely at random
			//cout << "selection size -> " << selection.card << endl;
			int r=maximum(selection.card,5);
			int neSize=minimum(r,selection.card);
			assert(neSize<=selection.card);
			Binom ne(selection.card,neSize);
			ne.select();
			int poSize=minimum(r,(complement.card)/2+1);
			assert(poSize<=complement.card);
			Binom po(complement.card,poSize);
			po.select();
// 	    				cout << "removing " << neSize << "  adding " << poSize << endl << flush;
			Set pos(n);
			Set neg(n);
			for (int l=0; l<poSize; l++)
			{
				assert(po[l]>=0);
				assert(po[l]<n);
				pos.add(complement.list[po[l]]);
			}
			for (int l=0; l<neSize; l++)
			{
				assert(ne[l]>=0);
				assert(ne[l]<n);
				neg.add(selection.list[ne[l]]);
			}
			Set pos2(neg);
			Set neg2(pos);
// 	    				cout << "current -> " << selection << endl << "add -> " << pos << endl << "rem -> " << neg << endl << flush;
	
// 	    				cout << "move to anti\n" << endl;
	   				//move to anti-thesis
			if (moveH3(pos,neg)) noIm=0;
// 					print();
// 	    				cout << "move back\n" << endl;
	    				//return to origin
// 					if (moveH3(pos2,neg2)) noIm=0;
// 					print();
	 
// 	    				cout<<"move to best"<<endl;
			moveTo(best);
// 					print();
		}
				//cout << endl;
	}
// 			for(int u=0;u<best.card;u++)
// 				cout<<best.list[u]<<" ";
// 			cout<<endl;
	return bestVal;
}

		void commit(int b)
{
   			 //assert(newCover[b]>0);
			//add be to selected sets
	bool r=selection.add(b);
	assert(r);
			//remove b from unselected sets
	r=complement.rem(b);
	assert(r);
			//decrement uncovered by b
	uncovered-=newCover[b];
			//increment cost by b
	cost+=c[b];
			//For elements in b
	for (int k=0; k<setCard[b]; k++)
	{
		int i=set[b][k];
				//add b to his coverers
		if (covered[i]->card>1) 
		{
			covered[i]->add(b);
			continue;
		}
				//add d to his coverers and 
		if (covered[i]->card==1) 
		{
					//decrement criritical element count
			disCover[covered[i]->list[0]]--;
			covered[i]->add(b);
			continue;
		}
				//increment critical item count of b
		disCover[b]++;
				//add b to his coverer
		covered[i]->add(b);
				
		for (int l=0; l<appearCard[i]; l++)
		{
			int j=appears[i][l];
					//decrement Newcover of other sets that contains elements of b
			newCover[j]--;
		}
	}
	assert(newCover[b]==0);
}

		void unCommit(int b)
{
	bool r=selection.rem(b);
	assert(r);
	r=complement.add(b);
	assert(r);

	cost-=c[b];
	for (int k=0; k<setCard[b]; k++)
	{
		int i=set[b][k];
		assert(covered[i]->card>0);
		if (covered[i]->card>2)
		{
			covered[i]->rem(b);
			continue;
		}
		if (covered[i]->card>1)
		{
			covered[i]->rem(b);
			assert(covered[i]->card==1);
			disCover[covered[i]->list[0]]++;
			continue;
		}
		assert(covered[i]->card==1);
		assert(covered[i]->list[0]==b);
		covered[i]->rem(b);
		disCover[b]--;
		uncovered++;
		for (int l=0; l<appearCard[i]; l++)
		{
			int j=appears[i][l];
			newCover[j]++;
		}
	}
	assert(disCover[b]==0);
}

		float upperBound(float* dis, Set& bSet, int it=1)
  		// computes upper bound by mixing heuristics according to 'dis'
  		// returns the minimum of 'it' runs
{
			//1,000,000 interval split according to dis
	long long* D = new long long[numberOfHeuristics];
	float acc=0.;
	for (int i=0; i<numberOfHeuristics; i++)
	{
		acc+=dis[i];
		D[i]=(int) minimum(acc*1000000.,1000000.);
	}
	D[numberOfHeuristics-1]=1000000;
	float bVal=INF;

	for (int p=0; p<it; p++)
	{
		reset();
		int b;
				//cout << endl;
		while (uncovered>0)
		{
			long long r=rand()%1000000;
			int x;
					//pick a heuristics
			for (x=0;x<numberOfHeuristics; x++)
				if (r<D[x]) break;
			assert(x<numberOfHeuristics);
					
					//select a set acc to heuristics
			switch(x)
			{
				case 0: b=h0(); break;
				case 1: b=h1(); break;
				case 2: b=h2(); break;
				case 3: b=h3(); break;
				case 4: b=h4(); break;
				case 5: b=h5(); break;
				case 6: b=h6(); break;
				case 7: b=h7(); break;
				case 8: b=h8(); break;
				case 9: b=h9(); break;
				case 10: b=h10(); break;
				case 11: b=h11(); break;
				case 12: b=h12(); break;
				case 13: b=h13(); break;
				case 14: b=h14(); break;
				case 15: b=h15(); break;
				case 16: b=h16(); break;
				case 17: b=h17(); break;
				case 18: b=h18(); break;
				case 19: b=h19(); break;
				case 20: b=h20(); break;
				case 21: b=h21(); break;
				case 22: b=h22(); break;
			}
					
			if (b==-1) //infeasible instance!
			{
				delete [] D;
				return -1;
			}
					//found a new set to add
			commit(b);
		}
		if (cost<bVal)
		{
					//update cost and selection
			bVal=cost;
			bSet=selection;
		}
	}
	delete [] D;
	return bVal;
}
};


#endif




// 			//Weak Antithesis
// 			int numCandids = selection.card/3;
// // 			cout<<"#candids: "<<numCandids<<endl;
// 			
// 			HHeap<int,float> remove(selection.card);
// 			for (int j=0; j<selection.card; j++)
// {
// 	remove.ins(selection.list[j],disCover[selection.list[j]]);
// }
// 			
// 			Set available = complement;
// 			
// 			//Remove numCandids sets acc. to least discover
// 			for(i=0;i<numCandids;i++)
// {
// 	cout<<"- "<<remove.min()<<" "<<remove.minValue()<<endl;
// 	unCommit(remove.min());
// 	remove.del();
// }
// 			
// 			cout<<"Now uncovered: "<<uncovered<<endl;
// 			HHeap<int,float> add(available.card);
// 			for (int j=0; j<available.card; j++)
// {
// 	add.ins(available.list[j],-1*newCover[available.list[j]]);
// }
// 		
// 			//Add sets until feasible acc. to most newCover
// 			while(uncovered > 0)
// {
// 	cout<<"Uncovered: "<<uncovered<<endl;
// 	cout<<"+ "<<add.min()<<" "<<-1*add.minValue()<<endl;
// 	commit(add.min());
// 	add.del();
// }

/*
			//Strong Antithesis
			int sizeA = selection.card/3;
			int sizeB = complement.card/3;
			
			cout<<sizeA<<" "<<sizeB<<endl;
			
			Binom a(selection.card,sizeA);
			a.select();
			
			Binom b(complement.card,sizeB);
			b.select();
			
			Set subsetA(sizeA);
			Set subsetB(sizeB);
			
			for(i=0;i<sizeA;i++)
{
// 				subsetA.add(selection.list[a[i]]);
	cout<<a[i]<<" "<<selection.list[a[i]]<<endl;
}
			
			for(j=0;j<sizeB;j++)
{
// 				subsetB.add(complement.list[b[i]]);
	cout<<b[i]<<" "<<complement.list[b[i]]<<endl;
}
			
			cout<<"a("<<subsetA.card<<") "<<flush;
			for(int i=0;i<subsetA.card;i++)
				cout<<subsetA.list[i]<<" ";
			
			cout<<"b("<<subsetB.card<<") "<<flush;
			for(int i=0;i<subsetB.card;i++)
				cout<<subsetB.list[i]<<" ";
			*/
			
						
// 			for (int l=0; l<neSize; l++)
// {
// 	assert(ne[l]>=0);
// 	assert(ne[l]<n);
// 	neg.add(selection.list[ne[l]]);
// }
// 			
// 			
// 			
// 			
// 			//Remove numCandids sets
// 			for(i=0;i<neg.card;i++)
// {
// 	cout<<"- "<<neg.list[i]<<endl;
// 	unCommit(neg.list[i]);
// }
// 			
// 			//Add sets until feasible acc. to most newCover
// 			while(uncovered > 0)
// {
// 	int mostCover = -1;
// 	int mostCoverSet = -1;
// 	for (int j=0; j<pos.card; j++)
// 	{
// 		if(newCover[pos.list[j]] > mostCover)
// 		{
// 			mostCover = newCover[pos.list[j]];
// 			mostCoverSet = pos.list[j];
// 		}
// 	}
// 					
// 	if(mostCover > 0)
// 	{
// 		cout<<"+ "<<mostCoverSet<<" "<<mostCover<<endl;
// 		commit(mostCoverSet);
// 		pos.rem(mostCoverSet);
// 	}
// 	else//If what we removed can not be covered by the anti thesis.
// 	{
// 		while(uncovered>0)
// 		{
// 			mostCover = -1;
// 			for (int j=0; j<complement.card; j++)
// 			{
// 				if(newCover[complement.list[j]] > mostCover)
// 				{
// 					mostCover = newCover[complement.list[j]];
// 					mostCoverSet = complement.list[j];
// 				}
// 			}
// 			cout<<"++ "<<mostCoverSet<<" "<<mostCover<<endl;
// 			commit(mostCoverSet);
// 		}
// 	}
// }
