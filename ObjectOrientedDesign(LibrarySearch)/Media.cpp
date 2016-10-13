#include<iostream>
#include<string>
#include"Media.h"

using namespace std;

Media::Media(string a,string b,string c,string d)
{
 this->call_number=a;
 this->title=b;
 this->subject=c;
 this->notes=d;
}

bool Media::compare_title(const string& x) const
{
size_t found=this->title.find(x);
  if (found!=std::string::npos)
    return true;
  else
  return false;

}

bool Media::compare_call_number(const string& x) const
{
size_t found=this->call_number.find(x);
  if (found!=std::string::npos)
    return true;
  else
  return false;

}

bool Media::compare_subject(const string& x) const
{
size_t found=this->subject.find(x);
  if (found!=std::string::npos)
    return true;
  else
  return false;
}

bool Media::compare_other(const string& x) const
{
}

void Media::display()const
{
}

Media::~Media()
{
}
