#ifndef FILM_H
#define FILM_H

#include<iostream>
#include<string>
#include "Media.h"

using namespace std;



class Film : public Media 
{
                                      
    private:
                
       string year;
       string director;  
      
        
    public:
       Film(string, string, string, string, string, string); 
       void display() const;   
       bool compare_other(const string&) const;
       
	
};

#endif
