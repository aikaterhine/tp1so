#include "../include/main.hpp"

using namespace std;

//VARS GLOBAIS
pthread_mutex_t lock_forno;
pthread_mutex_t lock_p[NTHREADS];
pthread_cond_t  cond_p[NTHREADS];
pthread_mutex_t lock_raj_liberou;
pthread_mutex_t lock_cout;
bool raj_liberou = false;

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
        case SHELDON:
          setName("Sheldon");
          break;
        case HOWARD:
          setName("Howard");
          break;
        case LEONARD:
          setName("Leonard");//Howard
          break;
        case RAJ:
          setName("Raj");
          break;
        case KRIPKE:
          setName("Kripke");
          break;
        case AMY:
          setName("Amy");
          break;
        case BERNADETTE:
          setName("Bernadette");
          break;
        case PENNY:
          setName("Penny");
          break;
        case STUART:
          setName("Stuart");
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
      srand48(time(NULL));
      int number = 3 + (drand48 () * 3);
      pthread_mutex_lock(&lock_cout);
      cout << _name << " voltou para o trabalho" << "\n";
      pthread_mutex_unlock(&lock_cout);
      sleep(number);
    }
};

//Variáveis globais
Person p[10];

//FUNCOES GLOBAIS

int prioridades(int idPessoa){
  int prior = -1;
  switch(idPessoa){
    case SHELDON:
      prior=LEONARD;
      break;
    case HOWARD:
      prior=SHELDON;
      break;
    case LEONARD:
      prior=HOWARD;
      break;
  }
  return prior;
}

void trata_prioridades(Person pessoa){

  int idPessoa = pessoa.getId();
  int prior = prioridades(idPessoa);

  if(prior!= -1){
    //tranca o forno
    pthread_mutex_lock(&lock_forno);

    pthread_mutex_lock(&lock_p[prior]);
    if(p[prior].getQuer_usar())
      pthread_mutex_unlock(&lock_forno);
    while(p[prior].getQuer_usar()){
      pthread_cond_wait(&cond_p[prior], &lock_p[prior]);

      pthread_mutex_lock(&lock_raj_liberou);
      if(raj_liberou){
        pthread_mutex_unlock(&lock_raj_liberou);
        break;
      }
    }

    pthread_mutex_lock(&lock_cout);
    cout << pessoa.getName() << " começa a esquentar algo" << "\n";
    pthread_mutex_unlock(&lock_cout);
    pthread_mutex_unlock(&lock_p[prior]);
  }
}

class Microwave {
	public:
		Microwave(){
    }

    void wait(Person &pessoa){
      pthread_mutex_lock(&lock_p[pessoa.getId()]);
      pessoa.setQuer_usar(true);
      pthread_mutex_unlock(&lock_p[pessoa.getId()]);

      pthread_mutex_lock(&lock_cout);
      cout << pessoa.getName() << " quer usar o forno." << "\n";
      pthread_mutex_unlock(&lock_cout);

      trata_prioridades(pessoa);
    }

    void release(Person &pessoa){
      pthread_mutex_lock(&lock_p[pessoa.getId()]);
      pessoa.setQuer_usar(false);
      pthread_cond_signal(&cond_p[pessoa.getId()]);
      pthread_mutex_unlock(&lock_p[pessoa.getId()]);

      pthread_mutex_unlock(&lock_forno);

      pthread_mutex_lock(&lock_cout);
      cout << pessoa.getName() << " vai comer." << "\n";
      pthread_mutex_unlock(&lock_cout);
    }

    void check(){
      if(p[SHELDON].getQuer_usar() and p[HOWARD].getQuer_usar() and p[LEONARD].getQuer_usar()){
        srand48(time(NULL));
        int nmr_pessoa = drand48 () * 2;
        int prior = prioridades(nmr_pessoa);

        if(prior!=-1){
          pthread_mutex_lock(&lock_raj_liberou);
          raj_liberou = true;
          pthread_mutex_unlock(&lock_raj_liberou);
          pthread_cond_signal(&cond_p[prior]); //liberando pessoa de acordo com numero sorteado
          pthread_mutex_lock(&lock_cout);
          cout << "Raj detectou um deadlock, liberando " << p[nmr_pessoa].getName() << "\n";
          pthread_mutex_unlock(&lock_cout);
        }
      }
    }
};

 //MAIN

int threads_repeat;

Microwave forno;
int thrds_terminated = 0;
pthread_mutex_t lock_thrds_terminated;

void* execute_threads(void* thread) {
  long my_thread = (long) thread;

  if(my_thread != RAJ){
    for (int i = 0; i < threads_repeat; i++) {
      forno.wait(p[my_thread]);
      p[my_thread].cook_something();
      forno.release(p[my_thread]);
      p[my_thread].eat();
      p[my_thread].work();

      if(i == threads_repeat - 1){
        pthread_mutex_lock(&lock_thrds_terminated);
        thrds_terminated++;
        pthread_mutex_unlock(&lock_thrds_terminated);

        pthread_mutex_lock(&lock_cout);
        cout << "--------------------------------------" << "\n";
        cout << p[my_thread].getName() << ": Terminou suas tarefas." << "\n";
        pthread_mutex_unlock(&lock_cout);
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
        pthread_mutex_lock(&lock_cout);
        cout << "--------------------------------------" << "\n";
        cout << p[my_thread].getName() << ": Terminou suas tarefas." << "\n";
        pthread_mutex_unlock(&lock_cout);
        return NULL;
      }
      pthread_mutex_unlock(&lock_thrds_terminated);
    }
  }
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
      perror("\nMutex lock_raj_liberou init has failed");
      return 1;
    }

    if (pthread_mutex_init(&lock_cout, NULL) != 0) {
      perror("\nMutex lock_cout init has failed");
      return 1;
    }

    if (pthread_mutex_init(&lock_thrds_terminated, NULL) != 0) {
        perror("\nMutex lock_thrds_terminated init has failed");
        return 1;
    }

    if (pthread_mutex_init(&lock_forno, NULL) != 0) {
        perror("\nMutex lock_forno init has failed");
        return 1;
    }

    for(i = 0; i < NTHREADS; i++){

      if (pthread_cond_init(&cond_p[i], NULL) != 0) {
        perror("\nCond cond_p init has failed");
        return 1;
      }

      if (pthread_mutex_init(&lock_p[i], NULL) != 0) {
        perror("\nMutex lock_p init has failed");
        return 1;
      }
    }

    thread_handles = (pthread_t*) malloc (NTHREADS*sizeof(pthread_t));

    for (thread = 0; thread < NTHREADS; thread++){
      error = pthread_create(&thread_handles[thread], NULL, execute_threads, (void*) thread);
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
    pthread_mutex_destroy(&lock_cout);
    return 0;
}
