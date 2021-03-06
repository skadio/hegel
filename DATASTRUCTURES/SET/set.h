
#ifndef SET_H
#define SET_H

class Set
{
  long* marked;
  long maxCard;

  void alloc(void)
  {
    marked = new long[maxCard];
    list = new long[maxCard];
  }
  
  void freeMem(void)
  {
    delete [] marked; marked=0;
    delete [] list; list=0;
  }

  void init(void)
  {
    long j;
    for (j=0; j<maxCard; j++)
      marked[j]=-1;
  }

public: 
  long* list;
  long card;

  Set(long mc) : maxCard(mc), card(0)
  {
    alloc();
    init();
  }

  ~Set(void)
  {
    freeMem();
  }

  Set(const Set& s) : maxCard(s.maxCard), card(s.card)
    {
      if (this==&s) return;
      alloc();
      for (long j=0; j<maxCard; j++)
	{
	  marked[j]=s.marked[j];
	  list[j]=s.list[j];
	}
    }

  long& operator[](const int& i)
  {
    assert(i>=0);
    assert(i<card);
    return list[i];
  }

  Set& operator=(const Set& s)
    {
      
      if (s.maxCard>maxCard) 
	{
	  freeMem();
	  maxCard=s.maxCard;
	  alloc();
	}
      long j;
      for (j=0; j<card; j++)
	marked[list[j]]=-1;
      card=s.card;
      for (j=0; j<s.card; j++)
	{
	  list[j]=s.list[j];
	  marked[list[j]]=j;
	}
      return *this;
    }

  bool add(long item)
    // adds an item to the set, returns false if item was in set before
  {
    assert(item>=0);
    assert(item<maxCard);
    if(marked[item]!=-1) return false;
    marked[item]=card;
    list[card++]=item;
    return true;
  }

  bool rem(long item)
    // removes an item, returns false if item was not in the set
  {
    /*
    if (card<10) 
      {
	cout << "removing " << item << endl;
	long i;
	for (i=0; i<card; i++)
	  cout << list[i] << " ";
	cout << endl;
	for (i=0; i<card; i++)
	  cout << marked[list[i]] << " ";
	cout << endl;
      }
    */
    assert(item>=0);
    assert(item<maxCard);
    if(marked[item]==-1) return false;
    list[marked[item]]=list[--card];
    marked[list[marked[item]]]=marked[item];
    marked[item]=-1;
    /*
    if (card<10) 
      {
	long i;
	for (i=0; i<card; i++)
	  cout << list[i] << " ";
	cout << endl;
	for (i=0; i<card; i++)
	  cout << marked[list[i]] << " ";
	cout << endl;
      }
    */
    return true;
  }

  void remAll(void)
  {
    long j;
    for (j=0; j<card; j++)
      marked[list[j]]=-1;
    card=0;
  }

  bool isInSet(long item)
  {
    if(marked[item]==-1) return false;
    return true;
  }

  void add(const Set& s)
  {
    assert(maxCard==s.maxCard);
    long i,item;
    for (i=0; i<s.card; i++)
      {
	item=s.list[i];
	if(marked[item]==-1)
	  {
	    marked[item]=card;
	    list[card++]=item;
	  }
      }
  }
};


ostream& operator<<(ostream& out, const Set& s)
{
  for (long i=0; i<s.card; i++)
    out << s.list[i] << " ";
  return out;
}


#endif

