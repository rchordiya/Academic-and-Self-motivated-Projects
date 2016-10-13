#ifndef MEDIA_H
#define MEDIA_H

#include<iostream>
#include<string>

using namespace std;

class Media
{

   public:
           bool compare_call_number(const string&) const;
           bool compare_title(const string&) const;
           bool compare_subject(const string&) const;
           virtual bool compare_other(const string&) const;
           virtual void display() const ;
           Media(string,string,string,string);
           virtual ~Media();
   
   protected:
           string call_number;
           string title;
           string subject;
           string notes;
 };  
 
 #endif
