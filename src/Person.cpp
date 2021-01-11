#include "../include/Person.hpp"

Person::Person(int userId, string userName){
  _name = userName;
  _id = userId;
}

Person::Person(){
}

void Person::setName(string userName){
  _name = userName;
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
