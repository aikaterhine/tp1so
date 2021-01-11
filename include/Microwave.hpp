#ifndef MICROWAVE_HPP
#define MICROWAVE_HPP

#include "../include/Person.hpp"

#include <string>

#include <iostream>
#include <mutex>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <math.h>

using namespace std;

class Microwave {
	private:
		/**
		* Variáveis globais.
		*/
		// Regiões críticas.

		pthread_mutex_t mutexMicrowave;

	public:
		Microwave();

    void wait(Person);
    void release(Person);
    void check();

};

#endif

/*
 int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);
 int pthread_mutex_lock(pthread_mutex_t *mutex);
 int pthread_mutex_unlock(pthread_mutex_t *mutex); int pthread_mutex_destroy(pthread_mutex_t *mutex);
 int pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr)
 int pthread_cond_signal(pthread_cond_t *cond);
 int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
 int pthread_cond_destroy(pthread_cond_t *cond);
*/
