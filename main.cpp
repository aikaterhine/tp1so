#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "include/Person.hpp"

int main (int argc, char * argv []){
  Person p = new Person(1, "Sheldon");
  printf(p.getName());


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
