#ifndef PERSON_HPP
#define PERSON_HPP

#include <string>

using namespace std;

  class Person {
    private:
      int _id;
      string _name;

    public:
      void setName(string);
      void setId(int);
      string getName();
      int getId();
    	Person(int, string);
      Person();

/*
      void cook_something(Person);
      void eat(Person);
      void sleep(int);
*/
  };

#endif // PERSON_HPP
