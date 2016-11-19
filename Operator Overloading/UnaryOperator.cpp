#include <iostream>

using namespace std;

class Summation
{
private:
  int x,y,z;
public:
  void display();
  void operator-();
  Summation()
  {
    x=10;
    y=20;
    z=30;
  }

};
void Summation::operator-()
{
  x=-x;
  y=-y;
  z=-z;
}
void Summation::display()
{
  cout<<x <<" "<<y<<" "<<z<<endl;
}
int main()
{
  Summation s;
  s.display();
  -s;
  s.display();
  return 0;
}
