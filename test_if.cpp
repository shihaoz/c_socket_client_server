#include <iostream>

using namespace std;

int hi(){
  return -2;
}
int main(){
  int a;
  if((a = hi()) == -1){
    cout << "hi\n";
  }
  else{
    cout << "no hi\n"; 
  }
}
