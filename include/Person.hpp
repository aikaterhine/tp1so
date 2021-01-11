#ifndef PERSON_HPP
#define PERSON_HPP

#include <string>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <stdio.h>

using namespace std;

  class Person {
    private:
      int _id;
      string _name;

    public:
      void setName(int);
      void setId(int);
      string getName();
      int getId();
    	Person(int, string);
      Person();

      void cook_something(Person);
      void eat(Person);
      void work(Person);
      void sleep(int);

  };

#endif // PERSON_HPP
