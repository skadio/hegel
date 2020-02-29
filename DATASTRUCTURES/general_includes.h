#ifndef GENERAL_INCLUDES_H
#define GENERAL_INCLUDES_H

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
//#include <time.h>
#include <sys/param.h>
#include <sys/times.h>
#include <sys/types.h>
#include <math.h>
#include <iostream>

#define PROGERROR(x) error(__FILE__, __LINE__, x, ENR)
#define RUNTIMEERROR(x) error(__FILE__, __LINE__, x, ENR)
#define RUNERROR(x) error(__FILE__, __LINE__, x, ENR)
#define bool unsigned char

#if ! defined(false)
  #define false 0
  #define true !false  
#endif

using namespace std;
typedef int (*CompareFct) (const void *, const void *);

const long ENR=999; 
const int INFTY=100000000;
const double EPSILON=1./INFTY;
//const int INFINITY=INFTY;

void error(const char* file, int line, const char* message, long exitNumber);
double now(long long* t=0);
double convert(long long t);


#if ! defined(false)
long round(double a);
#endif

template <class T>
T sqr(T a)
{
  return a*a;
}

template <class T>
int chooseDist(T* dis, int n)
{
  if (n<=0) return -1;
  long long* D = new long long[n];
  T acc=0.;
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

int moreInt(double a, double b);
int integer(double a);

template<class T, class S>
T maximum(const T& a, const S& b)
{
  if (a<b) return b;
  return a;
}

template<class T, class S>
T minimum(const T& a, const S& b)
{
  if (a>b) return b;
  return a;
}

template <class T>  
void copy(long l, T* c, T* o)
{
  for (long i=0;i<l;i++)
    c[i]=o[i];  
}

template <class T>
void matrixCopy(int n, int m, T** p, T** _p);

template <class T>
void matrixAlloc(int n, int m, T**& matrix)
{
  int i;
  matrix=new T*[n];
  for (i=0; i<n; i++)
    {
      matrix[i]=new T[m];
    }
}

template <class T>
void matrixFree(int n, T**& matrix)
{
  int i;
  for (i=0; i<n; i++)
    delete [] matrix[i];
  delete [] matrix;
  matrix = 0;
}

bool acceptProb(double p);

long triangularIndex(long a, long b, long n);


#endif
