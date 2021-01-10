#ifndef PERSON_HPP
#define PERSON_HPP

#include <string>
#include <stdarg.h>

using namespace std;

namespace Person {

  class Person {
    private:
      int id;
      string name;

    public:
      void setName(string);
      void setId(int);
      string getName();
      int getId();
    	Person(int, string);
      Person();

      void cook_something(Person);
      void eat(Person);
      void sleep(int);
  };

}

#endif // PERSON_HPP
