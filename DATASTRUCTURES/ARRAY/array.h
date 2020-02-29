#ifndef ARRAY_H
#define ARRAY_H

#include <general_includes.h>
#include <assert.h>
#include <iostream>
 
using namespace std;

template <class T>
class Array
{
  T* array;
  long size;
  long offset;

public:

  Array(long s, long o=0) : size(s), offset(o)
    {
      array = new T[size];
      if (!array)
	RUNTIMEERROR("Insufficient memory!");
    };
  Array(const Array<T>& a) : size(a.s), offset(a.o)
    {
      array = new T[size];
      if (!array)
	RUNTIMEERROR("Insufficient memory!");
      long k;
      for (k=0; k<size; k++)
	array[k]=a.array[k];
    };
  Array<T>& operator=(const Array<T>& a)
    {
      if (this==&a) return *this;
      offset = a.offset;
      if (size<a.size)
	{
	  size = a.size;
	  delete [] array;
	  array = new T[size];
	  if (!array)
	    RUNTIMEERROR("Insufficient memory!");
	}
      long k;
      for (k=0; k<a.size; k++)
	array[k]=a.array[k];
      return *this;
    }
  ~Array(void) {delete [] array;}; 
  T& operator[] (long i);
  void shrink(long newSize);
  long getSize(void) const { return size;};
};

template <class T>
inline T& Array<T>::operator[] (long i)
{
  i-=offset;
  assert(i>=0);
  if (i>=size)
    {
      //cout << "resizing!\n";
      long s=2*size;
      while(i>=s) s*=2;
      T* h = array;
      array = new T[s];
      if (!array)
	RUNTIMEERROR("Insufficient Memory!");
      long k;
      for(k=0; k<size; k++)
	array[k] = h[k];
      delete [] h;
      size = s;
    }
  return array[i];
}

template <class T>
void Array<T>::shrink(long shiftSize)
{
  if (shiftSize>=size/2) return;
  long s=size/2;
  while(shiftSize<s/2) s/=2;
  T* h = array;
  array = new T[s];
  if (!array)
    RUNTIMEERROR("Insufficient Memory!");
  long k;
  for(k=0; k<shiftSize; k++)
    array[k] = h[k];
  delete [] h;
  size = s;
}

#endif



