#ifndef BINOM_H
#define BINOM_H

#include <general_includes.h>

class Binom
{
  int n,k;
  int *subset;
  int *fullset; 

public:
  Binom(int _n, int _k) : n(_n), k(_k)
    {
      assert(n>=k);
      if (k>0) subset = new int[k];
      else subset = 0;
      fullset = new int[n];
      init();
    }
  ~Binom(void)
    {
      delete [] subset;
      delete [] fullset;
    }
  inline int operator[](int i)
    {
      assert((i>=0)&&(i<k));
      return subset[i];
    }
  inline bool next(void)
    {
      int i = k-1;
      while ((i>=0)&&(subset[i]==n-k+i)) i--;
      if (i<0) 
	{
	  init();
	  return false;
	}
      assert(subset[i]<n-k+i);
      assert((i==k-1)||(subset[i]<subset[i+1]-1));
      subset[i]++;
      for (++i; i<k; i++)
	subset[i]=subset[i-1]+1;
      return true;
    }
  inline void init(void)
    {
      for (int i=0; i<k; i++)
	subset[i]=i;
    }
  inline void exchange(int& i, int& j)
    {
      static int x;
      x=i;
      i=j;
      j=x;
    };
  inline void select(void)
    {
      int i,h;
      for (i=0; i<n; i++)
	fullset[i]=i;
      for (i=0; i<k; i++)
	{
	  exchange(fullset[i], fullset[i+rand()%(n-i)]);
	  subset[i]=fullset[i];
	}
      // now sort the result
      for (i=0; i<n; i++)
	fullset[i]=0;
      for (i=0; i<k; i++)
	fullset[subset[i]]=1;
      h=0;
      for (i=0; i<n; i++)
	if (fullset[i]) subset[h++]=i;
    }
};

#endif
