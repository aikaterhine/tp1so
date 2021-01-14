#include "../include/main.hpp"
#include "../include/vars_global.hpp"

 #define NTHREADS 2

 /*

1 - quer usar o forno
2 - esquentar algo no forno
3 -comer
4 - voltar ao trabalho
 */

int threads_repeat;

Microwave forno;

using namespace std;

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
  /*for (int i = 0; i < threads_repeat; i++) {
     code
    printf("\n Repetindo pela %dª vez\n", i+1);
    sleep(10);
  }*/
    long my_rank = (long) rank;
    if(my_rank != 8){
      forno.wait(p[my_rank]);
      p[my_rank].cook_something();
      forno.release(p[my_rank]);
      p[my_rank].eat();
      p[my_rank].work();
    }

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

    for(i = 0; i < NTHREADS; i++){
      
      if (pthread_cond_init(&forno.cond_p[i], NULL) != 0) {
        perror("\nCond init has failed");
        return 1;
      }

      if (pthread_mutex_init(&forno.lock_p[i], NULL) != 0) {
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

    printf("\nHello from the main thread\n");

    for (thread = 0; thread < NTHREADS; thread++){
      pthread_join(thread_handles[thread], NULL);
    }

    free (thread_handles);
    for(i = 0; i < NTHREADS; i++){
      pthread_cond_destroy(&forno.cond_p[i]);

      pthread_mutex_destroy(&forno.lock_p[i]);
    }
    
    pthread_mutex_destroy(&forno.lock_forno);

    return 0;

}
