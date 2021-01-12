#include "../include/main.hpp"

int thread_count;

using namespace std;

void *Hello(void* rank){
    long my_rank = (long) rank;
    Person p;
    p.setId(my_rank);

    cout << "\n" << p.getName() << "\n" << endl;

    cout << "\nHi from thread " << my_rank << " of " << thread_count << "\n" << endl;

    sleep(10);

    return NULL;
}

void *raj(void* rank){
    long my_rank = (long) rank;
    printf("\nHi from thread %ld of %d\n", my_rank, thread_count);
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

    long thread;

    pthread_t* thread_handles;

    thread_count = strtol(argv[1], NULL, 10);

    thread_handles = (pthread_t*) malloc (thread_count*sizeof(pthread_t));

    printf("numero de threads = %d", thread_count);

    for (thread = 0; thread < thread_count; thread++){
      pthread_create(&thread_handles[thread], NULL, Hello, (void*) thread);
    }

    printf("\nHello from the main thread\n");

    for (thread = 0; thread < thread_count; thread++){
      pthread_join(thread_handles[thread], NULL);
    }

    free (thread_handles);
    return 0;

}
