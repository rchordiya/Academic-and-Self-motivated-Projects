#include<iostream>
#include<string>
#include"Film.h"
#include"Media.h"
using namespace std;


Film :: Film( string a,string b,string c,string d, string e, string f) : Media(a, b, c, e)
{  
  this->director=d;  
  this->year=f; 
  
}

void Film::display() const
{
   cout<<"<------------------------------ Record From Film--------------------------------------------->"<<endl;
   cout<<"Call Number:"<<call_number<<"\n"<<"Title:"<<title<<"\n"<<"Subjects:"<<subject<<"\n"<<"Director:"<<director<<"\n"<<"Notes:"<<notes<<
  "\n"<<"Year:"<<year<<endl;
}


bool Film::compare_other(const string& x) const
{
  
  if (this->notes.find(x) != std::string::npos)
  {
     return true;
  } 
  else if(this->director.find(x) != std::string::npos)
   {
     return true;
   }
    else if(this->year.find(x) != std::string::npos) 
   {
     return true;
   }
   else 
   {
     return false;
   }
}

