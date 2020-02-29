#ifndef QUEUE_H
#define QUEUE_H

//************  NOT TESTED YET! ***************

#include <general_includes.h>
#include <ARRAY/array.h>
#include <assert.h>

template <class T>
class Queue
{
  Array<T> queue;
  Array<long> next;
  Array<long> deleted;
  long first, last;
  long size;
  long vacant; 

public:

  Queue(long maxSize=1000) : queue(maxSize), next(maxSize), deleted(maxSize)
    {
      reset();
    }
  ~Queue(void)
    {}
  void enq(const T& element)
    {
      if (vacant)
	{
	  assert(vacant>0);
	  assert(deleted[vacant-1]>=0);
	  assert(deleted[vacant-1]<queue.getSize());
	  queue[deleted[--vacant]]=element;
	  next[last]=deleted[vacant];
	  next[deleted[vacant]]=-1;
	  last=deleted[vacant];
	  if (size++==0) 
	    {
	      assert(first==-1);
	      assert(last==-1);
	      first=deleted[vacant];
	    }
	}
      else
	{
	  assert(vacant==0);
	  queue[size]=element;
	  next[last]=size;
	  next[size]=-1;
	  last=size;
	  if (size==0) 
	    {
	      assert(first==-1);
	      assert(last==-1);
	      first=size;
	    }
	  size++;
	}
    }
  bool deq(T& element)
    {
      if (!size) 
	{
	  assert(first==-1);
	  assert(last==-1);
	  return false;
	}
      element=queue[first];
      deleted[vacant++]=first;
      first=next[first];
      if (--size==0) 
	{
	  assert(first==-1);
	  last=-1;
	}
      return true;
    }
  void reset(void)
    {
      size=vacant=0;
      first=last=-1;
    }
  bool empty(void)
    {
      return !size;
    }
};

#endif



