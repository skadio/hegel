
#include <list.h>

int main(void)
{
  List<long> l;
  ListIterator<long> it(l);

  long i;
  for (i=0; i<20; i++)
    {
      long h = i+1;
      if (rand()%2)
	{
	  cout << "insert after current pointer, leaving pointer alone!\n";
	  l.insertAfter(h,it);
	}
      else
	{
	  cout << "insert after current pointer, advancing pointer!\n";	  
	  it = l.insertAfter(h,it);
	}
      l.operator<<(cout) << endl;
    }
  cout << "FIRST 20 DONE!\n";
  for (i=0; i<20; i++)
    {
      if (!it.ok()) break;
      if (rand()%2)
	{
	  cout << "advancing pointer to -> ";
	  ++it;
	  cout << (long) it << endl;
	}
      else 
	{
	  cout << "removing index -> " << (long)it << endl;
	  l.remove(it);
	}
      l.operator<<(cout) << endl;
    }
  cout << "DNE WITH REMOVING PASS THROUGH THE LIST\n";
  for (i=0; i<20; i++)
    {
      long h = i+1;
      cout << "inserting -> " << h << " after " << (long)it << endl;
      if (rand()%2)
	l.insertAfter(h,it);
      else
	{
	  cout << "advancing pointer!\n";
	  it = l.insertAfter(h,it);
	}
      l.operator<<(cout) << endl;
    }
  return 0;
}
