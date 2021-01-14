#include "../include/Microwave.hpp"
#include "../include/vars_global.hpp"

Microwave::Microwave(){
}

void Microwave::wait(Person pessoa){
  cout << pessoa.getName() << " quer usar o forno." << "\n";
  
  //checa se pode usar, usando um loop
  if(pessoa.getId() == 0){
    pthread_mutex_lock(&lock_p[pessoa.getId()]);
    pessoa.setQuer_usar(true);
  }
  else
  if(pessoa.getId() == 1){//trocar para 2, quando acabar os testes
    pthread_mutex_lock(&lock_p[0]);
    while(p[0].getQuer_usar()){
      pthread_cond_wait(&cond_p[0], &lock_p[0]);
    }
    pthread_mutex_lock(&lock_p[pessoa.getId()]);
    pessoa.setQuer_usar(true);
  }

  //tranca o forno
  pthread_mutex_lock(&lock_forno);
}

void Microwave::release(Person p){
  cout << p.getName() << " vai comer." << "\n";
  
  p.setQuer_usar(false);
  pthread_cond_signal(&cond_p[p.getId()]);
  pthread_mutex_unlock(&lock_p[p.getId()]);
  //destranca o forno
  pthread_mutex_unlock(&lock_forno);
}

void Microwave::check(){
  //cout << p.getName() << " vai comer" << "\n";
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
