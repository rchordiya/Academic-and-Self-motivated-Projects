#include<iostream>
#include<fstream>
#include<string.h>
#include"customfstream.h"
using namespace std;
int main(int argc,char*argv[])
{
  string str1;
  string str2;
  char* fileName;
  
  str1=argv[1];
  str2=argv[2];
  fileName=argv[3];
  
 /* cout<<"Enter the old string to be replaced "<<endl;
  cin>>str1;
  cout<<"Enter the new string"<<endl;
  cin>>str2;*/
  
  
  customfstream obj;
  if(str1.size()==str2.size())
  {
      obj.getfile1(str1,str2,fileName);
  }
  else if(str1.size()>str2.size())
  {
      obj.getfile2(str1,str2,fileName);
  }
  else 
  {
      obj.getfile3(str1,str2,fileName);
  }  
  return 0;
}



