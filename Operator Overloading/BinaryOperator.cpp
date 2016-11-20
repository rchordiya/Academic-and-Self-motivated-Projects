#include <iostream>
using namespace std;

class complex
{
private:
  float x,y;
public:
  complex()
  {}
  complex(float real,float img)
  {
    x=real;
    y=img;
  }
  void display();
  complex operator+(complex);


};
void complex::display()
{
  cout<<x<<"+j"<<y<<endl;
}
complex complex::operator+(complex c)
{
  complex temp;
  temp.x = x + c.x;
  temp.y= y + c.y;
  return (temp);
}

int main()
{
  complex C1,C2,C3;
  C1=complex(6.5,8.7);
  C2=complex(7.7,9.3);
  C3 = C1 + C2;
  C1.display();
  C2.display();
  C3.display();

  return 0;
}
