#ifndef PERMUTATION_H
#define PERMUTATION_H

#include <iostream>
#include "../general_includes.h"
#include <assert.h>


//enum bool {false = 0, true = 1};

class Permutation
{
private:
  const int _n;
  int x;
  int * _position;
  int * _permutation;
  
  bool perm(int i);
  
  void exchange(int& i, int& j)
    {
      x=i;
      i=j;
      j=x;
    };	
  
public:
  Permutation(int n);
  ~Permutation(void);
  
  void init(void);
  
  Permutation& operator++(void);
  int operator[](int i) const;
  void permute(void)
    {
      assert(_permutation);
      assert(_n);
      for (long i=0; i<_n-1; i++)
	{
	  x=i+(rand()%(_n-i));
	  if (i==x) continue;
	  assert(x>i);
	  assert(x<_n);
	  exchange(_permutation[i],_permutation[x]);
	}
    };
  bool next(void);  
  friend ostream& operator<< (ostream& os, Permutation& p);
};

#endif
