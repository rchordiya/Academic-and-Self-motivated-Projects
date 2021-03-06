#include<iostream>
#include<string>
#include"Periodic.h"
#include"Media.h"
using namespace std;


Periodic :: Periodic( string a,string b,string c,string d, string e, string f, string g, string h, string i,string j,string k,string l) : Media(a, b, c, i)
{  
  this->author=d;
  this->description=e;
  this->publisher=f;
  this->publishing_history=g;
  this->series=h; 
  this->related_titles=j;
  this->other_forms_of_title=k;
  this->govt_doc_number=l;
}

void Periodic::display() const
{
  cout<<"<------------------------------ Record From Periodic File--------------------------------------------->"<<endl;   
  cout<<"Call Number:"<<call_number<<"\n"<<"Title:"<<title<<"\n"<<"Subjects:"<<subject<<"Author:"<<author<<"\n"<<"Description:"<<description<<
  "\n"<<"Publisher:"<<publisher<<"\n"<<"Publishing History:"<<publishing_history<<"\n"<<"Series:"<<series<<"\n"<<"Notes:"<<notes<<"\n"<<"Related Titles:"<<related_titles<<"\n"<<" Other Forms Of Title:"<<other_forms_of_title<<"\n"<<"Govt Doc Number:"<<"\n"<<govt_doc_number<<endl;
}


bool Periodic::compare_other(const string& x) const
{
  if (this->description.find(x) != std::string::npos)
  {
     return true;
  }
   else if(this->series.find(x) != std::string::npos) 
  {
     return true;
  }
   else if(this->notes.find(x) != std::string::npos)
  {
    
     return true;
  } 
  else if(this->related_titles.find(x) != std::string::npos)
  {
     return true;
  }
   else 
  {
     return false;
  }
}


