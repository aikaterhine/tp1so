#include <iostream>
#include "include/Person.hpp"

#include "include/List.hpp"

using namespace Person;

Person(int userId, string userName){
  this.name = userName;
  this.id = userId;
}

Person(){
}

void setName(string userName){
	this.name = userName;
}

void setId(int userId){
	this.id = userId;
}

string getName(){
	return this.name;
}

int getId(){
	return this.id;
}
