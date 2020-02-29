#include "general_includes.h"

/**********************
TEMPLATES FUNCTIONS

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
void matrixCopy(int n, int m, T** p, T** _p)
{
  int i,j;
  for (i=0; i<n; i++)
    for (j=0; j<m; j++)
      p[i][j]=_p[i][j];
}

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

************************/

/*
char* itoa(int i)
{
  int j=1;
  int l=1;
  int a=i;
  if (a<0) a *= -1;
  do
    {
      a/=10;
      l++;
    }
  while (a>0);
  a=i;
  if (a<0) 
    {
      l++;
      a *= -1;
    }
  str[--l]=0;
  while (a>9)
    {
      assert(l-j>=0);
      str[l-j]=a%10+char('0');
      a/=10;
      j++;
    }
  if (i<0)
    {
      str[1]=a%10+char('0');
      str[0]=char('-');
    }
  else str[0]=a%10+char('0');
  return str;
}
*/



void error(const char* file, int line, const char* message, long exitNumber)
{
  if (file&&line)
    cerr << file << " " << line << endl << message << endl << flush;
  else 
    cerr << message << endl << flush;
  exit(exitNumber);
}

double now(long long *t) 
{    
  /*
  if (t) *t= clock();
  return (double) (clock()*1./CLOCKS_PER_SEC);
  */

  tms cpuTime;
  times(&cpuTime);
  //return (float)cpuTime.tms_utime / (float)CLK_TCK; // /100;
  return ((double) cpuTime.tms_utime)/HZ;
} 

double convert(long long t) 
{
  //cout << "CLOCK TICKS -> " << CLOCKS_PER_SEC << endl;
  return (double) (t*1./CLOCKS_PER_SEC);     // (float)CLK_TCK; // /100;
} 


#if ! defined(false)
long round(double a)
{
  return long(floor(a+.5));
}
#endif

int moreInt(double a, double b)
{
  return fabs(a-round(a)) < fabs(b-round(b));
}

int integer(double a)
{
  return int(fabs(a-round(a))<EPSILON);
}

bool acceptProb(double p)
{
  static long inter=1000000;
  assert(p>=0.);
  assert(p<=1.);
  long cutPoint=round(inter*p);
  if ((rand()%inter)<cutPoint)
    return true;
  return false;
}

long triangularIndex(long a, long b, long n)
{
  assert(a>=0);
  assert(a<n);
  assert(b>=0);
  assert(b<n);

  if (a>b)
    {
      long h=a;
      a=b;
      b=h;
    }
  assert(b>=a);
  return a*(n-1)-(a*(a-1))/2+b;
}
