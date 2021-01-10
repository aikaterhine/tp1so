#ifndef LIST_HPP
#define LIST_HPP

#include <string>
#include <stdarg.h>

using namespace std;
namespace List {

  class List {
    private:
      struct no{int valor; struct no *pro;};
      struct listaSimples{ struct no *ini; };

    public:
      void mostrar(struct listaSimples *inicio);
      void incluir(struct listaSimples *inicio, int v);
      void excluirInicio(struct listaSimples *inicio);
      void excluirEspecifico(struct listaSimples *inicio, int v);
      void pesquisar(struct listaSimples *inicio, int v);
      void alterar(struct listaSimples *inicio, int va, int vn);
      void excluirFim(struct listaSimples *inicio);
      void linhas();
  };

}

#endif // LIST_HPP
