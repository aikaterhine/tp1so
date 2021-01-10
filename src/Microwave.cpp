#include <iostream>
#include <thread>
#include <mutex>

#include "../src/Person.cpp"

using namespace std;

class Microwave {
  private:
    mutex m; //you can use std::lock_guard if you want to be exception safe
    int i = 0;

  public:

    Microwave(){
    }

    void wait(Person p){

    }

    void release(Person p){

    }

    void check(){

    }
};
