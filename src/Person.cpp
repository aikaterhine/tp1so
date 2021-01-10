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

/*
void cook_something(Person){

}

void eat(Person){

}

void sleep(int){

}
*/
