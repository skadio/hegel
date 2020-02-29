#ifndef LIST_H
#define LIST_H

// ----------------------------------------------------------------------
// list
// ----------------------------------------------------------------------

template <class T> class List;
template <class T> class ListPointer;
template <class T> class ListIterator;

#include<iostream>
#include<ARRAY/array.h>

using namespace std;

template <class T> 
class List
{
private:
  long length;
  Array<T> list;
  Array<long> next;
  Array<long> prev;
  long vacant;

public:

  List(long n=1000) : list(n), next(n), prev(n) { reset(); }
  ~List() {}

  ListIterator<T> insert(T e) 
    // inserts at the beginning of the list 
    // returns pointer to the new element
  {
    ListPointer<T> lp(0);
    return insertAfter(e,lp);
  }

  ListIterator<T> insertAfter(T e, ListPointer<T> p) 
    // inserts after the given element, or at the beginning of the list if 'p' not ok (=0)
    // returns pointer to the new element
  {
    if (vacant>0)
      {
	assert(vacant<list.getSize());
	list[vacant] = e;
	long h = prev[vacant];
	next[vacant] = next[p.ptr];
	prev[next[p.ptr]] = vacant;
	prev[vacant] = p.ptr;
	next[p.ptr]=vacant;
	ListIterator<T> lit(*this,vacant);
	vacant = h;
	length++;
	return lit;
      }
    list[++length] = e;
    next[length] = next[p.ptr];
    prev[length] = p.ptr;
    prev[next[p.ptr]]=length;
    next[p.ptr]=length;
    return ListIterator<T>(*this,length);
  }

  ListIterator<T> insertAfter(T e, ListIterator<T> it) 
    // inserts after the given element, to the beginning of the list if 'it' not ok
    // returns pointer to the new element
  {
    assert(it.l==this);
    //assert(it.ok()||length==0);
    if (vacant>0)
      {
	assert(vacant<list.getSize());
	list[vacant] = e;
	long h = prev[vacant];
	next[vacant] = next[it.ptr];
	prev[next[it.ptr]] = vacant;
	prev[vacant] = it.ptr;
	next[it.ptr]=vacant;
	ListIterator<T> lit(*this,vacant);
	vacant = h;
	length++;
	return lit;
      }
    list[++length] = e;
    next[length] = next[it.ptr];
    prev[length] = it.ptr;
    prev[next[it.ptr]]=length;
    next[it.ptr]=length;
    return ListIterator<T>(*this,length);
  }

  void remove(ListIterator<T>& it) 
  {
    assert(it.l==this);
    if (!it.ok()) return;
    long h=vacant;
    vacant=it.ptr;
    next[prev[it.ptr]]=next[it.ptr];
    prev[next[it.ptr]]=prev[it.ptr];
    ++it;
    prev[vacant]=h;
    --length;
  }


  void remove(ListPointer<T>& p) 
  {
    if (!p.ok(this)) return;
    long h=vacant;
    vacant=p.ptr;
    next[prev[p.ptr]]=next[p.ptr];
    prev[next[p.ptr]]=prev[p.ptr];
    p.ptr = next[p.ptr];
    prev[vacant]=h;
    --length;
  }

  void remove(long p) 
  {
    if (p==0) return;
    long h=vacant;
    vacant=p;
    next[prev[p]]=next[p];
    prev[next[p]]=prev[p];
    prev[vacant]=h;
    --length;
  }
  
  T& operator[](ListPointer<T> p) 
  {
    assert (p.ok(this));
    return list[p.ptr];
  }

  T& operator[](ListIterator<T> it) 
  {
    assert (it.ok());
    return list[it.ptr];
  }

  void reset(void) 
  {
    length = 0;
    next[0]=0;
    prev[0]=0;
    vacant=0;
  }

  char empty(void) const 
  {
    return length == 0;
  }

  long getSize(void) const
  {
    return length;
  }

  /*
  ostream& operator<<(ostream& os)
    {
      ListPointer<T> ip(*this);
      os << "List:\nindex - element - prev - next\n";
      while (ip.ok(*this))
	{
	  os << ip << " " << list[ip] << " " << prev[ip] << " " << next[ip] << endl << flush;
	  ip.advance(*this);
	}
      os << "vacant\n" << flush;
      long i=vacant;
      while (i!=0)
	{
	  os << i << " " << prev[i]<< endl << flush;
	  i=prev[i];
	}
      return os;
    }
  */

  ostream& operator<<(ostream& os)
    {
      ListIterator<T> li(*this);
      ListPointer<T> ip(*this);
      os << "List:\n";
      while (ip.ok(*this))
	{
	  assert (li.ok());
	  os << list[ip] << " ";
	  assert(list[ip]==list[(long)li]);
	  ip.advance(*this);
	  ++li;
	}
      assert(!li.ok());
      os << endl;
      return os;
    }

  friend class ListIterator<T>;
  friend class ListPointer<T>;

};
  
template <class T>
class ListIterator
{
  long ptr;
  List<T>* l;

 public:

  ListIterator(List<T>& _l, long _ptr=0) : ptr(_ptr), l(&_l)
    {
      if (!ptr) ptr = l->next[0];
      assert(ptr>=0);
    }
  ~ListIterator(void) {}

  void operator++(void)
  {
    assert(ptr>=0);
    assert(ptr<l->next.getSize());
    if (!ok()) return;
    ptr = l->next[ptr];
  }

  /*
  void operator--(void)
  {
    assert(ptr>=0);
    assert(ptr<l.prev.getSize());
    if (!ok()) return;
    ptr = l->prev[ptr];
    }
  */

  bool ok(void)
  {
    assert(ptr>=0);
    assert(ptr<l->list.getSize());
    return ptr!=0;
  }

  T& operator*(void)
  {
    assert(ok());
    return l->list[ptr];
  }

  operator long()
  {
    return ptr;
  }

  friend class List<T>;
  friend class ListPointer<T>;
};

template <class T>
class ListPointer
{
  long ptr;

 public:

  ListPointer(long p=0) : ptr(p) {}
  ListPointer(ListIterator<T> it) : ptr(it.ptr) {}
  ~ListPointer(void) {}

  bool ok(const List<T>& l)
  {
    assert(ptr>=0);
    assert(ptr<l.list.getSize());
    if (ptr==0) return false;
    return true;
  }

  void advance(List<T>& l)
  {
    assert(ok(l));
    ptr=l.next[ptr];
  }

  operator long()
  {
    return ptr;
  }

  friend class List<T>;
};


#endif
