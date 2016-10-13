#include<iostream>
#include<fstream>
#include"TrackingDeque.cpp"

using namespace std;

class Bufferfile
{
	
	private:
	     fstream f;
	     const char *x;
	          
	public:	      
	      int getter;	      
	      int putter;
	               
         bool trackeof;
			Bufferfile(const char*fileName);
			~Bufferfile();
			void put(char ch);
			Bufferfile& get(char &ch);
         bool eof();	
         void do_over(char ch);
         TrackingDeque<char> mybuffer;
         
         
	
};
