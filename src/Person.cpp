#include "../include/Person.hpp"

Person::Person(int userId, string userName){
  _name = userName;
  _id = userId;
}

Person::Person(){
}

void Person::setName(string username){
  _name = username;
}

void Person::setId(int userId){
	_id = userId;
  switch(userId){
    case 0:
      setName("Sheldon");
      break;
    case 1:
      setName("Amy");
      break;
    case 2:
      setName("Howard");
      break;
    case 3:
      setName("Bernadette");
      break;
    case 4:
      setName("Leonard");
      break;
    case 5:
      setName("Penny");
      break;
    case 6:
      setName("Stuart");
      break;
    case 7:
      setName("Kripke");
      break;
    case 8:
      setName("Raj");
      break;
  }
}

string Person::getName(){
	return _name;
}

int Person::getId(){
	return _id;
}

void Person::cook_something(Person p){
}

void Person::eat(Person p){

}

void Person::sleep(int time){
    sleep(time);
}

void Person::work(Person p){
  cout << p.getName() << " voltou para o trabalho\n"<< endl;
}
