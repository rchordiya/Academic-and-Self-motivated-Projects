#include<iostream>
#include "Bufferfile.h"

using namespace std;
class Streamsource
{
	private:
		Bufferfile bufObj;
		int replacement_count;
		int search_origin;
		int search_count;
		int string_length;
		string search_word;
		string replacement_word;
		bool search_get;
		bool replacement_get;
		
	
	public:
	   Streamsource(string,string,const char*);
	   ~Streamsource();
	   bool currently_replacing();
	   void matched_search_string();
	   void found_partial_match(int x, char ch);
	   bool eof();
	   Streamsource& get(char& ch);
	   void put(char ch);
};

