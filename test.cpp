#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
using namespace std;
int main() {
   ifstream f("a.txt"); //taking file as inputstream
   string str;
   if(f) {
      ostringstream ss;
      ss << f.rdbuf(); // reading data
      str = ss.str();
   }
   cout<<str;
}