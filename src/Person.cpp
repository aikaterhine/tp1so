#include <iostream>
#include<string>

//#include "../include/Person.hpp"
#include "../include/List.hpp"

using namespace std;
//using namespace PersonNS;

class Person {
  private:
    int _id;
    string _name;

  public:

    Person(int userId, string userName){
      _name = userName;
      _id = userId;
    }

    Person(){
    }

    void setName(string userName){
      _name = userName;
    }

    void setId(int userId){
    	_id = userId;
    }

    string getName(){
    	return _name;
    }

    int getId(){
    	return _id;
    }
};
