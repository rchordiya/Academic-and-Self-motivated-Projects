#include<iostream>
#include<string>
#include "SearchEngine.h"
#include"Media.h"
using namespace std;

int main()
{
  while(1)
{
   cout<<"Enter your choice"<<endl;
   cout<<"---------------------------------------"<<endl;   
   cout<<"1. Search by call number"<<endl;
   cout<<"2. Search by title"<<endl;
   cout<<"3. Search by subject"<<endl;
   cout<<"4. Search Other"<<endl;
   cout<<"5. Exit"<<endl;
   cout<<"---------------------------------------"<<endl;
   int n;
   cin>>n;
   string srch_str;
   SearchEngine search_obj;
   vector<Media*> results;
   cin.clear();
   cin.ignore();
   
   switch(n)
   {
      case 1:
             cout<<"Enter the call number"<<endl;
             getline(cin, srch_str);
             results = search_obj.search_by_call_number(srch_str);             
             for(int i=0;i<results.size();i++)
             {               
               results[i]->display();
               cout<<"------------------------------------------>"<<endl;
             }             
             
             break;
      case 2:
             
             cout<<"Enter the title"<<endl;
             getline(cin, srch_str);
             results = search_obj.search_by_title(srch_str);             
             for(int i=0;i<results.size();i++)
             {               
               results[i]->display();
               cout<<"------------------------------------------>"<<endl;
             }         
             
             break;
      case 3:
             cout<<"Enter the subject"<<endl;
             getline(cin, srch_str);
             results = search_obj.search_by_subjects(srch_str);             
             for(int i=0;i<results.size();i++)
             {               
               results[i]->display();
               cout<<"------------------------------------------>"<<endl;
             }
             
             
             break;
      case 4:
             cout<<"Enter string to search by other"<<endl;
             getline(cin, srch_str);             
             results = search_obj.search_by_other(srch_str);             
             for(int i=0;i<results.size();i++)
             {               
               results[i]->display();
               cout<<"------------------------------------------>"<<endl;
             }            
             
             break;
       case 5:
            
             return 0;      
      default:
             cout<<"Invalid option"<<endl;
             cin.clear();
             cin.ignore();
      
   }
  
   cout<<"Total Number of reports found ----------> "<<results.size()<<endl;
 }  
  return 0;   
      
}

