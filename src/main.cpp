#include "../include/main.hpp"

mutex m; //you can use std::lock_guard if you want to be exception safe
int i = 0;
void makeACallFromPhoneBooth(){
    m.lock();//man gets a hold of the phone booth door and locks it. The other men wait outside
      //man happily talks to his wife from now....
      std::cout << i << " Hello Wife" << std::endl;
      i++;//no other thread can access variable i until m.unlock() is called
      //...until now, with no interruption from other men
    m.unlock();//man lets go of the door handle and unlocks the door
}

int main (int argc, char * argv []){
  Person p;

}
