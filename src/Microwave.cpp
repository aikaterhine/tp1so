#include "../include/Microwave.hpp"

Microwave::Microwave(){
}

void Microwave::wait(Person p){
  cout << p.getName() << " quer usar o forno." << "\n";
}

void Microwave::release(Person p){
  cout << p.getName() << " vai comer." << "\n";
}

void Microwave::check(){
  //cout << p.getName() << " vai comer." << "\n";
}
