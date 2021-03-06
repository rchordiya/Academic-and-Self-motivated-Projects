#ifndef BOOK_H
#define BOOK_H
#include<iostream>
#include<string>
#include "Media.h"

using namespace std;

class Book : public Media 
{
                                      
    private:     
       string author;
       string description;
       string publisher;
       string city;
       string year;
       string series;     
      
        
     public:
       Book(string, string, string, string, string, string,string,string,string,string); 
       void display() const;   
       bool compare_other(const string&) const;
       
	
};

#endif
