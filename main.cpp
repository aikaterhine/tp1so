#include "../include/main.hpp"

using namespace std;

//VARS GLOBAIS
pthread_mutex_t lock_forno;
pthread_mutex_t lock_p[NTHREADS];
pthread_cond_t  cond_p[NTHREADS];
pthread_mutex_t lock_raj_liberou;
pthread_mutex_t lock_cout;
pthread_mutex_t lock_primeiro_do_casal[3];
bool raj_liberou = false;
int primeiro_do_casal[3];


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
          setName("Leonard");
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
    case STUART:
      prior=STUART;//como o stuart tem mais de um prior, a gente coloca ele mesmo como prior e trata nas funções
      break;
    case KRIPKE:
      prior=KRIPKE;//como o Kripke tem mais de um prior, a gente coloca ele mesmo como prior e trata nas funções
      break;
    default:
      perror("Prioridade não encontrada!");
  }
  return prior;
}

int casais(int idPessoa){
  int casal = -1;
  switch(idPessoa){
    case SHELDON:
      casal=AMY;
      break;
    case HOWARD:
      casal=BERNADETTE;
      break;
    case LEONARD:
      casal=PENNY;
      break;
    case BERNADETTE:
      casal=HOWARD;
      break;
    default:
      perror("Casal não encontrado!");
  }
  return casal;
}

int id_casal(int idPessoa){
  int id_casal = -1;
  switch(idPessoa){
    case SHELDON:
      id_casal=0;
      break;
    case AMY:
      id_casal=0;
      break;
    case HOWARD:
      id_casal=1;
      break;
    case BERNADETTE:
      id_casal=1;
      break;
    case LEONARD:
      id_casal=2;
      break;
    case PENNY:
      id_casal=2;
      break;
    default:
      perror("id_Casal não encontrado!");
  }

  return id_casal;
}

void trata_prioridades(Person pessoa){

  int idPessoa = pessoa.getId();
  int prior = prioridades(idPessoa);
  //cout << "--------------------------------------" << pessoa.getName() << ": Seu prior é: " << p[prior].getName() << "\n";
  if((prior!= -1) and (prior != STUART) and (prior != KRIPKE)){
    //tranca o forno
    pthread_mutex_lock(&lock_forno);

    int id_primeiro_casal;
    int casal = casais(idPessoa);

    pthread_mutex_lock(&lock_p[casal]);
    if(casal != -1){
      if(!p[casal].getQuer_usar()){
        pthread_mutex_lock(&lock_p[prior]);
        int casal_prior = casais(prior);
        pthread_mutex_lock(&lock_p[casal_prior]);
        
        if(p[prior].getQuer_usar() or p[casal_prior].getQuer_usar()){
          pthread_mutex_unlock(&lock_forno);

          while(p[prior].getQuer_usar() and p[casal_prior].getQuer_usar()){
            pthread_mutex_unlock(&lock_p[prior]);
            pthread_mutex_unlock(&lock_p[casal_prior]);

            sleep(1);

            pthread_mutex_lock(&lock_p[prior]);
            pthread_mutex_lock(&lock_p[casal_prior]);
          }
        
          while(p[casal_prior].getQuer_usar()){
            cout << "--------------------------------------" << pessoa.getName() << ": Esta esperando " << p[casal_prior].getName() << "\n";
            pthread_cond_wait(&cond_p[casal_prior], &lock_p[casal_prior]);

            pthread_mutex_lock(&lock_raj_liberou);
            if(raj_liberou){
              raj_liberou = false;
              pthread_mutex_unlock(&lock_raj_liberou);
              break;
            }
            else
              pthread_mutex_unlock(&lock_raj_liberou);
          }
          
          while(p[prior].getQuer_usar()){
            cout << "--------------------------------------" << pessoa.getName() << ": Esta esperando " << p[prior].getName() << "\n";
            pthread_cond_wait(&cond_p[prior], &lock_p[prior]);

            pthread_mutex_lock(&lock_raj_liberou);
            if(raj_liberou){
              raj_liberou = false;
              pthread_mutex_unlock(&lock_raj_liberou);
              break;
            }
            else
              pthread_mutex_unlock(&lock_raj_liberou);
          }
          cout << "--------------------------------------" << pessoa.getName() << ": Parou de esperar " << p[prior].getName() << "\n";
          
          pthread_mutex_lock(&lock_forno);
        }  
      }
      else
      {
        id_primeiro_casal = id_casal(idPessoa);
        if(id_primeiro_casal != -1){
          pthread_mutex_lock(&lock_primeiro_do_casal[id_primeiro_casal]);
          if(primeiro_do_casal[id_primeiro_casal] != idPessoa)
            while(p[casal].getQuer_usar())
              pthread_cond_wait(&cond_p[casal], &lock_p[casal]);
          
          pthread_mutex_unlock(&lock_primeiro_do_casal[id_primeiro_casal]);
        }
      }
    }
    pthread_mutex_lock(&lock_cout);
    cout << pessoa.getName() << " começa a esquentar algo" << "\n";
    pthread_mutex_unlock(&lock_cout);
    if(!p[casal].getQuer_usar() and (casal != -1))
      pthread_mutex_unlock(&lock_p[prior]);
    pthread_mutex_unlock(&lock_p[casal]);

    id_primeiro_casal = id_casal(idPessoa);
    if(id_primeiro_casal != -1){
      pthread_mutex_lock(&lock_primeiro_do_casal[id_primeiro_casal]);
      primeiro_do_casal[id_primeiro_casal] = -1;
      pthread_mutex_unlock(&lock_primeiro_do_casal[id_primeiro_casal]);
    }
  }
  else
  if((prior == STUART) or (prior == KRIPKE)){
    //tranca o forno
    pthread_mutex_lock(&lock_forno);

    if(prior == KRIPKE){
      pthread_mutex_lock(&lock_p[STUART]);

      if(p[STUART].getQuer_usar()){
        pthread_mutex_unlock(&lock_forno);

        while(p[STUART].getQuer_usar())
          pthread_cond_wait(&cond_p[STUART], &lock_p[STUART]);

        pthread_mutex_lock(&lock_forno);
      }
    }

    pthread_mutex_lock(&lock_p[SHELDON]);
    pthread_mutex_lock(&lock_p[HOWARD]);
    pthread_mutex_lock(&lock_p[LEONARD]);  
    if(p[SHELDON].getQuer_usar() or p[HOWARD].getQuer_usar() or p[LEONARD].getQuer_usar()){
      cout << "--------------------------------------" << pessoa.getName() << ": Algum dos 3 quer usar o forno " << "\n";
      pthread_mutex_unlock(&lock_forno);

      //os 3 querem usar(espera o Raj resolver a situação)
      while(p[SHELDON].getQuer_usar() and p[HOWARD].getQuer_usar() and p[LEONARD].getQuer_usar()){
        pthread_mutex_unlock(&lock_p[SHELDON]);
        pthread_mutex_unlock(&lock_p[HOWARD]);
        pthread_mutex_unlock(&lock_p[LEONARD]);

        sleep(1);

        pthread_mutex_lock(&lock_p[SHELDON]);
        pthread_mutex_lock(&lock_p[HOWARD]);
        pthread_mutex_lock(&lock_p[LEONARD]);
      }
      cout << "--------------------------------------" << pessoa.getName() << ": Saimos do deadlock ou nao teve deadlock " << "\n";
      //um par quer usar
      while(p[SHELDON].getQuer_usar() and p[HOWARD].getQuer_usar()){
        cout << "--------------------------------------" << pessoa.getName() << ": Par Sheldon e Howard " << "\n";
        pthread_mutex_unlock(&lock_p[SHELDON]);

        sleep(1);

        pthread_mutex_lock(&lock_p[SHELDON]);
      }

      while(p[HOWARD].getQuer_usar() and p[LEONARD].getQuer_usar()){
        cout << "--------------------------------------" << pessoa.getName() << ": Par Howard e Leonard " << "\n";
        pthread_mutex_unlock(&lock_p[HOWARD]);

        sleep(1);

        pthread_mutex_lock(&lock_p[HOWARD]);
      }
 
      while(p[LEONARD].getQuer_usar() and p[SHELDON].getQuer_usar()){//Leonard eventualmente conseguirá o seu lock
        cout << "--------------------------------------" << pessoa.getName() << ": Par Leonard e Sheldon " << "\n";
        pthread_mutex_unlock(&lock_p[LEONARD]);                      //após isto, ele irá liberar o forno e enviar um sinal para quem o espera
                                                                     //Por enquanto, Sheldon será com certeza a pessoa que recebe esse sinal e ganha o lock
        sleep(1);                                                    //Sheldon irá esquentar algo no forno, mas precisa do seu lock para liberar o forno         
                                                                     //Esta situação se repete em todos os whiles de pares
        pthread_mutex_lock(&lock_p[LEONARD]);                             
      }                                                              
      
      // um dos 3 quer usar
      while(p[SHELDON].getQuer_usar())
        pthread_cond_wait(&cond_p[SHELDON], &lock_p[SHELDON]);

      while(p[HOWARD].getQuer_usar())
        pthread_cond_wait(&cond_p[HOWARD], &lock_p[HOWARD]);

      while(p[LEONARD].getQuer_usar())
        pthread_cond_wait(&cond_p[LEONARD], &lock_p[LEONARD]);

      pthread_mutex_lock(&lock_forno);
    }
    pthread_mutex_lock(&lock_cout);
    cout << pessoa.getName() << " começa a esquentar algo" << "\n";
    pthread_mutex_unlock(&lock_cout);

    pthread_mutex_unlock(&lock_p[SHELDON]);
    pthread_mutex_unlock(&lock_p[HOWARD]);
    pthread_mutex_unlock(&lock_p[LEONARD]);
    if(prior == KRIPKE)
      pthread_mutex_unlock(&lock_p[STUART]);
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
   
      int id_primeiro_casal = id_casal(pessoa.getId());
      
      if(id_primeiro_casal != -1){
        pthread_mutex_lock(&lock_primeiro_do_casal[id_primeiro_casal]);
        if(primeiro_do_casal[id_primeiro_casal] == -1)
          primeiro_do_casal[id_primeiro_casal] = pessoa.getId();
        pthread_mutex_unlock(&lock_primeiro_do_casal[id_primeiro_casal]);
      }      

      trata_prioridades(pessoa);
    }

    void release(Person &pessoa){
      pthread_mutex_lock(&lock_p[pessoa.getId()]);
      pessoa.setQuer_usar(false);
      pthread_cond_broadcast(&cond_p[pessoa.getId()]);
      pthread_mutex_unlock(&lock_p[pessoa.getId()]);

      pthread_mutex_unlock(&lock_forno);

      pthread_mutex_lock(&lock_cout);
      cout << pessoa.getName() << " vai comer." << "\n";
      pthread_mutex_unlock(&lock_cout);
    }

    void check(){
      pthread_mutex_lock(&lock_p[SHELDON]);
      pthread_mutex_lock(&lock_p[HOWARD]);
      pthread_mutex_lock(&lock_p[LEONARD]);
      if(p[SHELDON].getQuer_usar() and p[HOWARD].getQuer_usar() and p[LEONARD].getQuer_usar()){
        srand48(time(NULL));
        int nmr_pessoa = drand48 () * 2;
        int prior = prioridades(nmr_pessoa);

        if(prior!=-1){
          pthread_mutex_lock(&lock_raj_liberou);
          raj_liberou = true;
          pthread_mutex_unlock(&lock_raj_liberou);
          pthread_cond_broadcast(&cond_p[prior]); //liberando pessoa de acordo com numero sorteado
          pthread_mutex_lock(&lock_cout);
          cout << "Raj detectou um deadlock, liberando " << p[nmr_pessoa].getName() << "\n";
          pthread_mutex_unlock(&lock_cout);
        }
      }
      pthread_mutex_unlock(&lock_p[SHELDON]);
      pthread_mutex_unlock(&lock_p[HOWARD]);
      pthread_mutex_unlock(&lock_p[LEONARD]);
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

    for(i = 0; i < 3; i++)
      primeiro_do_casal[i] = -1;
    

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

    for(i= 0; i < 3; i++){
      if (pthread_mutex_init(&lock_primeiro_do_casal[i], NULL) != 0) {
        perror("\nMutex lock_primeiro_do_casal init has failed");
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

    for(i= 0; i < 3; i++){
      pthread_mutex_destroy(&lock_primeiro_do_casal[i]);
    }

    pthread_mutex_destroy(&lock_forno);
    pthread_mutex_destroy(&lock_thrds_terminated);
    pthread_mutex_destroy(&lock_raj_liberou);
    pthread_mutex_destroy(&lock_cout);
    return 0;
}
