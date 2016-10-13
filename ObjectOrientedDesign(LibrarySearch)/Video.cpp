#include<iostream>
#include<string>
#include"Video.h"
#include"Media.h"
using namespace std;


Video :: Video( string a,string b,string c,string d, string e, string f, string g, string h) : Media(a, b, c, f)
{    
  this->description=d;
  this->distributor=e;
  this->series=g;
  this->label=h;  
}

void Video::display() const
{
  cout<<"<------------------------------ Record From Video--------------------------------------------->"<<endl;
  cout<<"Call Number:"<<call_number<<"\n"<<"Title:"<<title<<"\n"<<"Subjects:"<<subject<<"\n"<<"Description:"<<description<<
  "\n"<<"Distributor:"<<distributor<<"\n"<<"Notes:"<<notes<<"\n"<<"Series:"<<series<<"\n"<<"Label:"<<label<<endl;
}


bool Video::compare_other(const string& x) const
{  
  if (this->description.find(x) != std::string::npos)
  {
     return true;
  } 
  else if(this->distributor.find(x) !=std::string::npos) 
  {
     return true;
  }
   else if(this->notes.find(x) != std::string::npos)
  {
     return true;
  } 
  else
   {
     return false;
   }
}


