#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H
#include "Film.h"
#include<string>
#include<vector>
#include "Book.h"
#include "Media.h"
#include"Periodic.h"
#include"Video.h"


using namespace std;

class SearchEngine {
    public:
        SearchEngine();
        ~SearchEngine();
        vector<Media*> search_by_title(const string& ss) const;       
        vector<Media*> search_by_call_number(const string& ss) const;
        vector<Media*> search_by_subjects(const string& ss) const;
        vector<Media*> search_by_other(const string& ss) const;
                               
    private:
       
         vector<Media*> CardCatalog;
        
        
};

#endif





