#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include "Person.cpp"

#include <thread>
#include <mutex>

std::mutex m;//you can use std::lock_guard if you want to be exception safe
int i = 0;


void makeACallFromPhoneBooth()
{
    m.lock();//man gets a hold of the phone booth door and locks it. The other men wait outside
      //man happily talks to his wife from now....
      std::cout << i << " Hello Wife" << std::endl;
      i++;//no other thread can access variable i until m.unlock() is called
      //...until now, with no interruption from other men
    m.unlock();//man lets go of the door handle and unlocks the door
}

int main (int argc, char * argv []){
/*Person p;
  p.setId(1);
  p.setName("Sheldon");

  std::cout << p.getId() << " " << p.getName() << "\n";*/

  //This is the main crowd of people uninterested in making a phone call

   //man1 leaves the crowd to go to the phone booth
   std::thread man1(makeACallFromPhoneBooth);
   //Although man2 appears to start second, there's a good chance he might
   //reach the phone booth before man1
   std::thread man2(makeACallFromPhoneBooth);
   //And hey, man3 also joined the race to the booth
   std::thread man3(makeACallFromPhoneBooth);

   man1.join();//man1 finished his phone call and joins the crowd
   man2.join();//man2 finished his phone call and joins the crowd
   man3.join();//man3 finished his phone call and joins the crowd

	return 1;
}


  /*
	int n_valores = 0,
		n_threads = 0,
		i = 0;

	int* args = (int *) malloc(sizeof(int));
	pthread_t* threads = (pthread_t *) malloc(sizeof(pthread_t));

	pthread_mutex_init(&lock, NULL);

	printf("Digite a quantidade de números a processar: ");
	scanf("%d", &n_valores);
	printf("Digite a quantidade de threads a criar: ");
	scanf("%d", &n_threads);

	max = n_valores;
	qtd_threads = n_threads;
	args = (int *) realloc(args, n_threads * sizeof(int));
	threads = (pthread_t *) realloc(threads, n_threads * sizeof(pthread_t));
	for (i = 0; i < n_threads; i++) {
		args[i] = i;
		pthread_create(&(threads[i]), NULL, thread_fn, &(args[i]));
	}
	for (i = 0; i < n_threads; i++) {
		pthread_join(threads[i], NULL);
	}

	pthread_mutex_destroy(&lock);

	printf("O número total de números primos encontrados foi: %d\n", n_primos);*/
