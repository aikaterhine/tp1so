#ifndef MICROWAVE_HPP
#define MICROWAVE_HPP

#include <string>
#include <stdarg.h>

#include "../src/Person.cpp"

using namespace std;
namespace Microwave {

  class Microwave {
    private:
      int id;
      string name;

    public:
      void wait(Person);
      void release(Person);
      void check();
  };

}

#endif // MICROWAVE_HPP
