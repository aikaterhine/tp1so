#include "../include/main.hpp"
#include "../include/vars_global.hpp"

 #define NTHREADS 4

 #define SHELDON 0
 #define AMY 1
 #define HOWARD 2
 #define BERNADETTE 3
 #define LEONARD 4
 #define PENNY 5
 #define STUART 6
 #define KRIPKE 7
 #define RAJ 8

 /*

1 - quer usar o forno
2 - esquentar algo no forno
3 -comer
4 - voltar ao trabalho
 */

using namespace std;

//CLASSES, VARS e FUNCOES GLOBAIS
pthread_mutex_t lock_forno;
pthread_mutex_t lock_p[NTHREADS];
pthread_cond_t  cond_p[NTHREADS];
bool raj_liberou = false;
pthread_mutex_t lock_raj_liberou;

//CLASSES
class Person {
    private:
      int _id;
      string _name;
      bool _quer_usar;

    public:
      Person(int userId, string userName){
  _name = userName;
  _id = userId;
  _quer_usar = false;
}

  Person(){
    _quer_usar = false;
  }

  void setName(string username){
    _name = username;
  }

  void setId(int userId){
  	_id = userId;
    switch(userId){
      case 0:
        setName("Sheldon");
        break;
      case 1:
        setName("Howard");//Amy
        break;
      case 2:
        setName("Leonard");//Howard
        break;
      case 3:
        setName("Raj");//Bernadette
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

  void setQuer_usar(bool quer_usar){
    _quer_usar = quer_usar;
  }

  string getName(){
  	return _name;
  }

  int getId(){
  	return _id;
  }

  bool getQuer_usar(){
    return _quer_usar;
  }

  void cook_something(){
    sleep(1);
  }

  void eat(){
    srand48(time(NULL));
    int number = 1 + (drand48 () * 2);
    sleep(number);
  }

  void work(){
    int number = 3 + (drand48 () * 3);
    cout << _name << " voltou para o trabalho" << "\n";
    sleep(number);
  }

};

//Variáveis globais
Person p[10];

//FUNCOES GLOBAIS
void trata_prioridades(Person pessoa){
  //tranca o forno
  pthread_mutex_lock(&lock_forno);

  if(pessoa.getId() == 0){
    pthread_mutex_lock(&lock_p[2]);//trocar o numero 2 depois dos testes
    if(p[2].getQuer_usar())
      pthread_mutex_unlock(&lock_forno);
    //cout << "--------------------------------------------------" << pessoa.getName() << " - " << p[2].getName() << "quer_usar pós PRIORIDADE: " << p[2].getQuer_usar() << "\n";
    while(p[2].getQuer_usar()){//trocar o numero 2 depois dos testes
      pthread_cond_wait(&cond_p[2], &lock_p[2]);//trocar o numero 2 depois dos testes
      
      pthread_mutex_lock(&lock_raj_liberou);
      if(raj_liberou){
        pthread_mutex_unlock(&lock_raj_liberou);
        break;
      }
    }

    cout << pessoa.getName() << " começa a esquentar algo" << "\n";
    pthread_mutex_unlock(&lock_p[2]);//trocar o numero 2 depois dos testes
  }
  else
  if(pessoa.getId() == 1){//trocar para 2, quando acabar os testes
    pthread_mutex_lock(&lock_p[0]);
    if(p[0].getQuer_usar())
      pthread_mutex_unlock(&lock_forno);

    //cout << "--------------------------------------------------" << pessoa.getName() << " - " << p[0].getName() << "quer_usar pós PRIORIDADE: " << p[0].getQuer_usar() << "\n";

    while(p[0].getQuer_usar()){
      pthread_cond_wait(&cond_p[0], &lock_p[0]);

      pthread_mutex_lock(&lock_raj_liberou);
      if(raj_liberou){
        pthread_mutex_unlock(&lock_raj_liberou);
        break;
      }
    }


    cout << pessoa.getName() << " começa a esquentar algo" << "\n";

    pthread_mutex_unlock(&lock_p[0]);
  }
  else
  if(pessoa.getId() == 2){//trocar para 4, quando acabar os testes
    pthread_mutex_lock(&lock_p[1]);//trocar para 4, quando acabar os testes
    if(p[1].getQuer_usar())
      pthread_mutex_unlock(&lock_forno);

    //cout << "--------------------------------------------------" << pessoa.getName() << " - " << p[1].getName() << " quer_usar pós PRIORIDADE: " << p[1].getQuer_usar() << "\n";

    while(p[1].getQuer_usar()){//trocar para 4, quando acabar os testes
      pthread_cond_wait(&cond_p[1], &lock_p[1]);//trocar para 4, quando acabar os testes

      pthread_mutex_lock(&lock_raj_liberou);
      if(raj_liberou){
        pthread_mutex_unlock(&lock_raj_liberou);
        break;
      }
    }


    cout << pessoa.getName() << " começa a esquentar algo" << "\n";

    pthread_mutex_unlock(&lock_p[1]);
  }
}

class Microwave {
	private:
		/**
		* Variáveis globais.
		*/
		// Regiões críticas.

		//pthread_mutex_t lock;

/*
		int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);
		int pthread_mutex_lock(pthread_mutex_t *mutex);
		int pthread_mutex_unlock(pthread_mutex_t *mutex);

		int pthread_mutex_destroy(pthread_mutex_t *mutex);
		int pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr);
		int pthread_cond_signal(pthread_cond_t *cond);
		int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
		int pthread_cond_destroy(pthread_cond_t *cond);
		*/

	public:
		Microwave(){
    }

void wait(Person &pessoa){
  pthread_mutex_lock(&lock_p[pessoa.getId()]);
  pessoa.setQuer_usar(true);
  pthread_mutex_unlock(&lock_p[pessoa.getId()]);

  cout << pessoa.getName() << " quer usar o forno." << "\n";
  //cout << "--------------------------------------------------" << pessoa.getName() << " - " << pessoa.getId() << " quer_usar depois \"quer usar forno\": " << pessoa.getQuer_usar() << "\n";
  
  trata_prioridades(pessoa);
}

void release(Person &pessoa){
  pthread_mutex_lock(&lock_p[pessoa.getId()]);
  pessoa.setQuer_usar(false);
  pthread_cond_signal(&cond_p[pessoa.getId()]);
  pthread_mutex_unlock(&lock_p[pessoa.getId()]);

  pthread_mutex_unlock(&lock_forno);

  cout << pessoa.getName() << " vai comer." << "\n";
}

void check(){
  //cout << "--------------------------------------------------" << "Variaveis quer_usar: " << p[0].getQuer_usar() << " " << p[1].getQuer_usar() << " " << p[2].getQuer_usar() << "\n";
  if(p[0].getQuer_usar() and p[1].getQuer_usar() and p[2].getQuer_usar())//lembrar de trocar os números quando acabar os testes(dentro do if também)
  {
    //int nmr_pessoa = drand48 () * 2;
    pthread_mutex_lock(&lock_raj_liberou);
    raj_liberou = true;
    pthread_mutex_unlock(&lock_raj_liberou);
    pthread_cond_signal(&cond_p[2]);//liberando o sheldon(numero 0)
    cout << "Raj detectou um deadlock, liberando " << p[0].getName() << "\n";
  }
}

/*
int Microwave::pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);
int Microwave::pthread_mutex_lock(pthread_mutex_t *mutex);
int Microwave::pthread_mutex_unlock(pthread_mutex_t *mutex);

int Microwave::pthread_mutex_destroy(pthread_mutex_t *mutex);
int Microwave::pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr)
int Microwave::pthread_cond_signal(pthread_cond_t *cond);
int Microwave::pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
int Microwave::pthread_cond_destroy(pthread_cond_t *cond);
*/

};











 //MAIN

int threads_repeat;

Microwave forno;
int thrds_terminated = 0;
pthread_mutex_t lock_thrds_terminated;


void *Hello (void* rank){
    long my_rank = (long) rank;
    Person p;
    Microwave m;
    p.setId(my_rank);

    cout << "\nHi from thread " << my_rank << " of " << NTHREADS << "\n" << endl;

    //sleep(10);

    return NULL;
}

void* trythis(void* rank)
{
  long my_rank = (long) rank;
  if(my_rank != 3){//trocar para 8 de volta
    for (int i = 0; i < threads_repeat; i++) {
      forno.wait(p[my_rank]);
      p[my_rank].cook_something();
      forno.release(p[my_rank]);
      p[my_rank].eat();
      p[my_rank].work();

      if(i == threads_repeat - 1){
        pthread_mutex_lock(&lock_thrds_terminated);
        thrds_terminated++;
        pthread_mutex_unlock(&lock_thrds_terminated);
        cout << p[my_rank].getName() << ": Terminou suas tarefas." << "\n";
        return NULL;
      }
    }
  }
  else{
    while(1){
      sleep(5);
      forno.check();

      pthread_mutex_lock(&lock_thrds_terminated);
      if(thrds_terminated == NTHREADS - 1){
        cout << "Numero de threads terminadas: " << thrds_terminated << "\n";
        cout << p[my_rank].getName() << ": Terminou suas tarefas." << "\n";
        return NULL;
      }
      pthread_mutex_unlock(&lock_thrds_terminated);
    }
  }
  cout << "Alguma coisa está errada." << "\n";
  return NULL;
}


void *raj(void* rank){
    long my_rank = (long) rank;
    printf("\nHi from thread %ld of %d\n", my_rank, NTHREADS);
    return NULL;
}

int main (int argc, char * argv []){

    int error;
    long thread;

    int i;
    for(i = 0; i < 10; i++){
      p[i].setId(i);
    }

    pthread_t* thread_handles;

    threads_repeat = strtol(argv[1], NULL, 10);
    
    if (pthread_mutex_init(&lock_raj_liberou, NULL) != 0) {
      perror("\nMutex init has failed");
      return 1;
    }


    if (pthread_mutex_init(&lock_thrds_terminated, NULL) != 0) {
        perror("\nMutex init has failed");
        return 1;
    }
    
    if (pthread_mutex_init(&lock_forno, NULL) != 0) {
        perror("\nMutex init has failed");
        return 1;
    }

    for(i = 0; i < NTHREADS; i++){
      
      if (pthread_cond_init(&cond_p[i], NULL) != 0) {
        perror("\nCond init has failed");
        return 1;
      }

      if (pthread_mutex_init(&lock_p[i], NULL) != 0) {
        perror("\nMutex init has failed");
        return 1;
      }
    }

    thread_handles = (pthread_t*) malloc (NTHREADS*sizeof(pthread_t));

    printf("Numero de threads = %d\n", NTHREADS);

    for (thread = 0; thread < NTHREADS; thread++){
      error = pthread_create(&thread_handles[thread], NULL, trythis, (void*) thread);
      if (error != 0)
        perror("\nThread can't be created");
    }

    for (thread = 0; thread < NTHREADS; thread++){
      pthread_join(thread_handles[thread], NULL);
    }


    free (thread_handles);
    for(i = 0; i < NTHREADS; i++){
      pthread_cond_destroy(&cond_p[i]);

      pthread_mutex_destroy(&lock_p[i]);
    }
    
    pthread_mutex_destroy(&lock_forno);
    pthread_mutex_destroy(&lock_thrds_terminated);
    pthread_mutex_destroy(&lock_raj_liberou);

    return 0;

}
