#ifndef STACK_H
#define STACK_H

#include <general_includes.h>
#include <ARRAY/array.h>
#include <assert.h>

template <class T>
class Stack
{
  Array<T> stack;
  long size;

public:

  Stack(long maxSize=1000) : stack(maxSize)
    {
      size=0;
    }
  ~Stack(void)
    {}
  void push(const T& element)
    {
      stack[size++]=element;
    }
  bool pop(T& element)
    {
      if (!size) return false;
      element=stack[--size];
      return true;
    }
  void reset(void)
    {
      size=0;
    }
  bool empty(void)
    {
      return !size;
    }
};

#endif



