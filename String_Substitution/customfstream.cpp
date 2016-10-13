 #include<iostream>
 #include<fstream>
 #include<string.h>
 #include"customfstream.h"
 #include<unistd.h>
 //std::fstream file("myfile.txt", std::ios::out|std::ios::in);
 customfstream::customfstream()
 {
   getter=0;
   putter=0;
 }
 
 
 void customfstream::getfile1(string oldstring,string newstring,char* fileName)
 {
  oldstr=oldstring;
  newstr=newstring;
  file.open(fileName, std::ios::out|std::ios::in);  
//  char ch;
  int i=1; 
  int k=0; 
  //int matchSize=0;
  bool found=false;
  char y;
      
   while(!file.eof())
   {      
         char h=get(); 
                 
         if(h!=oldstr[0])
         {            
            put(h);
         }
         else
         {
            
            if(oldstr.size()==1 && newstr.size()==1)
            {
               put(newstr[0]);
            }
            else
            {
               
               for(k=1;k<(int)oldstr.size();k++)
               {        
                         
                  y=get();
                                
                  if(y==oldstr[i])
                  {                     
                     i++;
                     found=true;                     
                  }
                  else if(y==oldstr[0])
                  {
                     for(int m=0;m<k;m++)
                     {
                        put(oldstr[m]);
                     }
                     k=0;
                     i=1;
                  }
                  else
                  {                                        
                     found=false;
                     break;
                  }
               }
               if(found)
               {
                  for(int u=0;u<(int)newstr.size();u++)
                  {  
                                        
                     put(newstr[u]);
                     
                     i=1;
                  }
               }
               else
               {
                  for(int t=0;t<k;t++)
                  {                                
                     put(oldstr[t]);
                     i=1;
                        
                  }
                  put(y);
               }
            }
         }
        
          
      }
  
   
 }
 
 void customfstream::getfile2(string oldstring,string newstring,char* fileName)
 {
   oldstr=oldstring;
  newstr=newstring;
   file.open(fileName, std::ios::out|std::ios::in);  
  //char ch;
  int i=1; 
  int k=0; 
  int count=1;
  int temp=0;
  char y;
  //int matchSize=0;
  bool found=false;
  
      
   while(!file.eof())
   {      
         
         char h=get();          
                
         if(h!=oldstr[0])
         {            
               
                if(file.eof())
               {
                  
                  truncate(fileName,putter);
                  break;
               }
               else if(count==(int)oldstr.size())
               {
                  while(temp<count)
                  {
                     put(h);
                     temp++;
                  }
                  truncate(fileName,putter);
                  break;
               }
               
               
                         
               put(h);
               
               
            
         }
         else
         {              
            for(k=1;k<(int)oldstr.size();k++)
            {        
                         
                y=get();                                                
               if(y==oldstr[i])
               {                     
                  i++;
                  count++;
                  found=true;                     
               }
               else if(y==oldstr[0])
               {
                  for(int m=0;m<k;m++)
                  {
                     put(oldstr[m]);
                  }
                  k=0;
                  i=1;
               }
               else
               {                    
                  found=false;
                  break;
               }
            }
            if(found)
            {
               for(int u=0;u<(int)newstr.size();u++)
               {                                        
                  put(newstr[u]);                    
                  i=1;
               }
               
            }
            else
            {
               for(int t=0;t<k;t++)
               {                                
                  put(oldstr[t]);
                  i=1;
                       
               }
               put(y);
            }
            
         }
        
          
      }
  
   truncate(fileName,putter);
 }
 void customfstream::getfile3(string oldstring,string newstring,char* fileName)
 {
   oldstr=oldstring;
   newstr=newstring;
   file.open(fileName, std::ios::out|std::ios::in);  
   int matchSize=0;      
   while(!file.eof())
   {      
      char h=get();  
      if(h==oldstr[matchSize])
      {
         matchSize++;
         while(matchSize < (int)oldstr.size())
         {
            char ch=get();
            if(ch==oldstr[matchSize])
            {
               matchSize++;
            }
         }
         
         if(matchSize==(int)oldstr.size()) //complete/successful match
         {
            for(int i=0; i<(int)newstr.size(); i++)
            {
               
               put(newstr[i]);
               
            }
           
         }
         else 
         {
      
            for(int i=0; i<matchSize; i++)
            {
               
               put(oldstr[i]);
            }
         }
       }
       else
       {
           
         put(h);
       }
     
          
   }

 
 
   
 }
 
char customfstream::get()
{
  char ch;
  if(!mybuffer.empty())
  {
     
    ch=mybuffer.front();
        
    mybuffer.pop_front();
 
  
  }
  else
  {
   
   file.seekg(getter);  
   ch=file.get();  
   getter++;    
  
  }
  return ch;
}

void customfstream::put(char& ch)
{
  
  if(getter==putter && (oldstr.size()<newstr.size()))
  {
    //cout<<"value of getter and putter in put in get=put "<<getter<<" "<<putter<<endl;
    char c=file.get();
    if(!file.eof())    
    mybuffer.push_back(c);    
    getter++;
    
  }
   if(file.eof() && (oldstr.size()<newstr.size())){
   //cout<<"asasdasd"<<ch<<file.tellp();   
      file.clear();
      file.seekp(putter);
      file.put(ch);
      file.setstate(std::ios::eofbit);
      putter++;
      }
      else{
   file.seekp(putter);  
   
   file.put(ch);  
   putter++;
   }
     
   return;
  
}
customfstream::~customfstream()
{
   
}

