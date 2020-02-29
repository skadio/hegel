#include "permutation.h"

bool Permutation::perm(int i)
{
  int j;
  bool r;
  
  if (i == _n)
    {
      //permute();
      return false;
    }
  
  if (_position[i] + 1 == _n - i)
    {
      // Folge ohne i permutieren
      r = perm(i + 1);
      // cout << "rec" << * this << endl;
      for (j = _n - i - 1; j > 0; j--) 
	{
	  _permutation[j] = _permutation[j - 1];
	  _position[_permutation[j]] = j;
	}
      _permutation[0] = i;
      _position[i] = 0;
      // cout << "rec" << * this << endl;
      return r;
    }
  
  // Zahl i um eins nach rechts schieben
  _permutation[_position[i]] = _permutation[(_position[i] + 1) % (_n - i)];
  _position[_permutation[_position[i]]] = _position[i];
  _position[i] = (_position[i] + 1) % (_n - i);
  _permutation[_position[i]] = i;
  return true;
}

Permutation::Permutation(int n) : _n(n)
{
  _permutation = new int[_n];
  _position = new int[_n];
  init();
}

void Permutation::init(void)
{
  int i;
  for (i = 0; i < _n; i++)
    {
      _permutation[i] = i;
      _position[i] = i;
    }
  //permute();
}

Permutation::~Permutation(void)
{
  delete[] _position;
  delete[] _permutation;
}

Permutation& Permutation::operator++(void)
{
  if (!perm(0)) init();
  return * this;
}

bool Permutation::next(void)
{
  return perm(0);
}

int Permutation::operator[](int i) const
{
  return _permutation[i];
}

ostream& operator<< (ostream& os, Permutation& p)
{
  int i;
  
  os << "[ ";
  for (i = 0; i < p._n; i++)
    os << p._permutation[i] << " ";
  os << "]";
  
#ifndef NDEBUG
  os << " [ ";
  for (i = 0; i < p._n; i++)
    os << p._position[i] << " ";
  os << "]";
#endif
  
  return os;
}
