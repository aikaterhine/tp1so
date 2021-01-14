#include "../include/Person.hpp"

Person::Person(int userId, string userName){
  _name = userName;
  _id = userId;
  _quer_usar = false;
}

Person::Person(){
  _quer_usar = false;
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
      setName("Howard");//Amy
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

void Person::setQuer_usar(bool quer_usar){
  _quer_usar = quer_usar; 
}

string Person::getName(){
	return _name;
}

int Person::getId(){
	return _id;
}

bool Person::getQuer_usar(){
  return _quer_usar;
}

void Person::cook_something(){
  cout << _name << " começa a esquentar algo" << "\n";
  sleep(1);
}

void Person::eat(){
  srand48(time(NULL));
  int number = 1 + (drand48 () * 2);
  sleep(number);
}

void Person::work(){
  int number = 3 + (drand48 () * 3);
  cout << _name << " voltou para o trabalho" << "\n";
  sleep(number);
}
