#include "../include/Microwave.hpp"

Microwave::Microwave(){
}

void Microwave::wait(Person p){
  cout << p.getName() << " quer usar o forno." << "\n";
}

void Microwave::release(Person p){
  cout << p.getName() << " vai comer." << "\n";
}

void Microwave::check(){
  //cout << p.getName() << " vai comer." << "\n";
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
