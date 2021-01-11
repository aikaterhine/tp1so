#include "../include/Person.hpp"

Person::Person(int userId, string userName){
  _name = userName;
  _id = userId;
}

Person::Person(){
}

void Person::setName(int userId){
  switch(userId){
    case 0:
      _name = "Sheldon";
      break;
    case 1:
      _name = "Amy";
      break;
    case 2:
      _name = "Howard";
      break;
    case 3:
      _name = "Bernadette";
      break;
    case 4:
      _name = "Leonard";
      break;
    case 5:
      _name = "Penny";
      break;
    case 6:
      _name = "Stuart";
      break;
    case 7:
      _name = "Kripke";
      break;
    case 8:
      _name = "Raj";
      break;
  }
}

void Person::setId(int userId){
	_id = userId;
}

string Person::getName(){
	return _name;
}

int Person::getId(){
	return _id;
}

void cook_something(Person p){
}

void eat(Person p){
  srand48(time(NULL));
  int number = drand48 () * 10;
  cout << number << endl;
  // tem q pausar a thread por esse tempo
}

void sleep(int time){

}

void work(Person p){

}
