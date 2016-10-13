#include<iostream>
#include"Streamsource.h"

using namespace std;
int main(int argc,char*argv[])
{
  string str1;
  string str2;
  char* fileName;
  
  str1=argv[1];
  str2=argv[2];
  fileName=argv[3];
  
  Streamsource stringObj(str1, str2, fileName);
  char c;
  int match_count=0;
  while(!stringObj.get(c).eof())
  {
    // cout<<"in while main "<<endl;
     if(!stringObj.currently_replacing() && c==str1[match_count])
     {
        match_count++;
        if(match_count < (int)str1.size())
        continue;
     }
     
     if(match_count == (int)str1.size())
     {
        stringObj.matched_search_string();
        match_count=0;
     }
     
     else if (match_count >0)
     {
        stringObj.found_partial_match(match_count,c);
        match_count=0;
     }
     
     else
     {
        stringObj.put(c);
     }
  }
return 0; 
}

