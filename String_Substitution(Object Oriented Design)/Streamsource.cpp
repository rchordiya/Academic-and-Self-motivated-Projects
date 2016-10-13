#include<iostream>
#include<string>
#include<string.h>
#include "Streamsource.h"

using namespace std;
Streamsource::Streamsource(string s1,string s2, const char* fileName) 
: bufObj(fileName),replacement_count(0),search_origin(0),search_count(0),string_length(0),search_word(s1),replacement_word(s2)
{

}

Streamsource& Streamsource::get(char& ch)
{
   if(search_count>0)
   {
      ch= search_word[string_length-search_count--];
      search_get=true;
   }
   
   else if( replacement_count >0)
   {
      ch=replacement_word[string_length - replacement_count--];
      replacement_get=true;
   }
   
   else
   {
      search_get=false;
      replacement_get=false;
      bufObj.get(ch);
   }
   return *this;
   
  
}


void Streamsource::found_partial_match(int count, char ch)
{
   if(search_get)
   {
      search_origin++;
   }
   
   else
   {
      search_origin=0;
      string_length=count;
      bufObj.do_over(ch);
   }
   
   put(search_word[search_origin]);
   search_count=string_length-1-search_origin;
  
}

bool Streamsource:: currently_replacing()
{
   
   return replacement_get; 

}

void Streamsource::matched_search_string()
{
   string_length = replacement_word.size();
   replacement_count = replacement_word.size();
   
   
}


bool Streamsource::eof()
{
   if(search_get)
   {
      return false;
   }
   else if(replacement_get)
   {
      return false;
   }
   
   else
   {
      return bufObj.eof();
   }
   

}



void Streamsource:: put(char ch)
{
   bufObj.put(ch);
}
	   
Streamsource::~Streamsource()
{
}
 
	   	   
	   
	   
	   
