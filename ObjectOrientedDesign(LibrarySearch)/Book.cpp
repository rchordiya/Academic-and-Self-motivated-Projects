#include<iostream>
#include<string>
#include"Book.h"
#include"Media.h"
using namespace std;


Book :: Book( string a,string b,string c,string d, string e, string f, string g, string h, string i,string j) : Media(a, b, c, j)
{  
  this->author=d;
  this->description=e;
  this->publisher=f;
  this->city=g;
  this->year=h;
  this->series=i;
  
}

void Book::display() const
{  
  cout<<"<------------------------------ Record From Book --------------------------------------------->"<<endl; 
  cout<<"Call Number:"<<call_number<<"\n"<<"Title:"<<title<<"\n"<<"Subjects:"<<subject<<"Author:"<<author<<"\n"<<"Description:"<<description<<
  "\n"<<"Publisher:"<<publisher<<"\n"<<"City:"<<city<<"\n"<<"Year:"<<year<<"\n"<<"Series:"<<series<<"\n"<<"Notes:"<<notes<<endl;
}


bool Book::compare_other(const string& x) const
{
  if (this->description.find(x) != std::string::npos)
  {
     return true;
  } 
  else if(this->year.find(x) != std::string::npos) 
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


