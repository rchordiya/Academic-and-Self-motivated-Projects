 #include<iostream>
 #include<fstream>
 #include<string.h>
 #include<string>
 #include"TrackingDeque.cpp"
 using namespace std;
class customfstream
{
   public:
         void getfile1(string x,string y,char* z);
         void getfile2(string x,string y,char* z);
         void getfile3(string x,string y,char* z);
         void emptybuf();
         char get();
         void put(char& ch);
         int getter;
         int putter;
         string newstr;
         string oldstr;
        // bool replaced;
         char* fileName;
         customfstream();
         ~customfstream();
         fstream file;
         TrackingDeque<char> mybuffer;
};
