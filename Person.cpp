#include <iostream>
using namespace std;

class Person {
public:
	Person(int, string);

  int id;
	string name;

	void setName(string);
  void setId(int);
};

Person::Person(int userId, string userName){
  name = userName;
  id = userId;
}

void Person::setName(string userName){
	name = userName;
}

void Person::setId(int userId){
	id = userId;
}
