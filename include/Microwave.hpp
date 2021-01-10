#ifndef MICROWAVE_HPP
#define MICROWAVE_HPP

#include <string>
#include <stdarg.h>

#include "include/Person.hpp"

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
