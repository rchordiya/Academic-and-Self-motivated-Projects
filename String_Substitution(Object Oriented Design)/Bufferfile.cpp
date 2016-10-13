#include<iostream>
#include<string>
#include<string.h>
#include<unistd.h>
#include"Bufferfile.h"

using namespace std;

Bufferfile::Bufferfile(const char* fileName):f(fileName,ios::in|ios::out),x(fileName),getter(0),putter(0),trackeof(false)
{	
}

void Bufferfile::put(char ch)
{
    if (putter == getter)
    {
        if(!trackeof) 
        {
           char s;
           f.seekg(getter);
    	     f.get(s);    	  
    	     getter ++;
           mybuffer.push_back(s);
           
        }
        f.seekp(putter);

       if (f.tellg()==-1 && f.eof())
       {
          f.clear();
          trackeof = true;
 //       f.setstate(std::ios::eofbit);
       }

          f.put(ch); 
          putter++;
    

    }
    
    else
    {
     /*   if(f.tellg()==-1  )
       {
          f.clear();       
          f.seekp(putter);
          f.put(ch);
          f.setstate(std::ios::eofbit);
          //putter++;
       }*/
       f.seekp(putter);
       if (f.tellg()==-1 && f.eof())
       {
          f.clear();
          trackeof = true; 
       }
       f.put(ch); 
       putter++;
    }

}
Bufferfile& Bufferfile::get(char &ch)
{   
  
  
    if (!mybuffer.empty())
    {
        ch = mybuffer.front();
        mybuffer.pop_front();
       // cout<<"que elements "<<ch<<endl;
    }
    else
    {
        f.seekg(getter);
        f.get(ch);
      
         getter ++;
    }
    if (f.tellg()==-1 && f.eof())
    {
        f.clear();
        trackeof = true;
   //     f.setstate(std::ios::eofbit);
    }
    return *this; 

}
bool Bufferfile::eof()
{
  if(!mybuffer.empty() )
  {
     return false;
  }
  else
  {
     return trackeof;
  }

}

void Bufferfile::do_over(char ch)
{
   mybuffer.push_front(ch);
}

Bufferfile::~Bufferfile()
{
   //const char *x=f.c_str();
   truncate(x,putter);
}

