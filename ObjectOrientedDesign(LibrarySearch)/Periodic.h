#ifndef Periodic_H
#define Periodic_H
#include<iostream>
#include<string>
#include "Media.h"

using namespace std;

class Periodic : public Media 
{
                                      
    private:     
       string author;
       string description;
       string publisher;
       string publishing_history;       
       string series;     
       string related_titles;
       string other_forms_of_title;
       string govt_doc_number;
        
       public:
       Periodic(string, string, string, string, string, string,string,string,string,string,string,string); 
       void display() const;   
       bool compare_other(const string&) const;
       
	
};

#endif
