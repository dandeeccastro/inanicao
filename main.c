#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

#define NTHREADS 4
#define BOOKSIZE 100

int book[BOOKSIZE];

void * Leitora ( void * arg ){}
void * Escritora ( void * arg ){}

int main (int argc, char *argv[]) {
	int i; pthread_t threads[NTHREADS];

	printf("Inicio da main\n");

	for (i = 0; i < NTHREADS; i++){
		if (i%2) {
			pthread_create( threads[i], NULL, Leitora, NULL);
		} else {
			pthread_create( threads[i], NULL, Escritora , NULL);
		}
	}

	printf("Main gerou todas as threads\n");

	for (i = 0; i < NTHREADS; i++)
		pthread_join(threads[i],NULL);

	return 0;
}
