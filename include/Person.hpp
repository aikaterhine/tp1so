#ifndef PERSON_HPP
#define PERSON_HPP

#include <string>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <unistd.h>
#include <stdio.h>

using namespace std;

  class Person {
    private:
      int _id;
      string _name;
      bool _quer_usar;

    public:
      void setName(string);
      void setId(int);
      void setQuer_usar(bool);
      string getName();
      int getId();
      bool getQuer_usar();
    	Person(int, string);
      Person();
      void cook_something();
      void eat();
      void work();

  };

#endif // PERSON_HPP
