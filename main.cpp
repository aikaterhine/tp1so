#include "../include/main.hpp"

 #define NTHREADS 9

int threads_repeat;
pthread_mutex_t lockM;

using namespace std;

void *Hello (void* rank){
    long my_rank = (long) rank;
    Person p;
    p.setId(my_rank);

    cout << "\nHi from thread " << my_rank << " of " << NTHREADS << "\n" << endl;

    sleep(10);

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
    pthread_mutex_lock(&lockM);

    unsigned long i = 0;
    printf("\n Job %ld has started\n", my_rank);

    for (i = 0; i < (0xFFFFFFFF); i++);

    printf("\n Job %ld has finished\n", my_rank);

    pthread_mutex_unlock(&lockM);

    return NULL;
}


void *raj(void* rank){
    long my_rank = (long) rank;
    printf("\nHi from thread %ld of %d\n", my_rank, NTHREADS);
    return NULL;
}

int main (int argc, char * argv []){
  /*struct Node *last = NULL;

  last = insertInEmpty(last, 30);
  last = insertAtBegin(last, 20);
  last = insertAtBegin(last, 10);
  last = insertAtEnd(last, 40);
  last = insertAtEnd(last, 60);
  last = insertAfter(last, 50,40 );
  cout<<"The circular linked list created is as follows:"<<endl;
  traverseList(last);
  deleteNode(&last,10);*/

    int error;
    long thread;

    pthread_t* thread_handles;

    threads_repeat = strtol(argv[1], NULL, 10);

    if (pthread_mutex_init(&lockM, NULL) != 0) {
        perror("\nMutex init has failed");
        return 1;
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
    pthread_mutex_destroy(&lockM);

    return 0;

}
