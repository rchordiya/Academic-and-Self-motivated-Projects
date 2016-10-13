#include<iostream>
#include<deque>

using namespace std;


template<class TYPE>
class TrackingDeque: public deque<TYPE> 
{

private:
  int max_size;
  
public:
   void push_back(TYPE);
   void push_front(TYPE);
   TrackingDeque();
   ~TrackingDeque();

};

template<class TYPE>
TrackingDeque<TYPE>::TrackingDeque():max_size(0)
{
}


template<class TYPE>
void TrackingDeque<TYPE>::push_back(TYPE x)
{
   deque<TYPE>::push_back(x);
  // cout<<"IN PUSHBACK"<<x<<"ok"<<endl;
   int s=deque<TYPE>::size();
   if(s>max_size)
   {
      max_size=s;
   }
   
   
   //max_size++;
}

template<class TYPE>
void TrackingDeque<TYPE>::push_front(TYPE x)
{
   deque<TYPE>::push_front(x);
    int s=deque<TYPE>::size();
   if(s>max_size)
   {
      max_size=s;
   }
  // max_size++;
}

template<class TYPE>
TrackingDeque<TYPE>::~TrackingDeque()
{
  cout<<"--> "<<max_size<<" <--"<<endl;
}













