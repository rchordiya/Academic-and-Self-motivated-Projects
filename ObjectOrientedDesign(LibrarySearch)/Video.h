#ifndef VIDEO_H
#define VIDEO_H
#include<iostream>
#include<string>
#include "Media.h"

using namespace std;

class Video : public Media 
{
                                      
    private:
       string description;
       string distributor;      
       string series;
       string label;       
     
      
        
       public:
       Video(string, string, string, string, string, string,string,string); 
       void display() const;   
       bool compare_other(const string&) const;
       
	
};

#endif
