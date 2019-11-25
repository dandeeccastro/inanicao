#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

#define NTHREADS 4
#define BOOKSIZE 100

int book;

void EntraLeitura(){
	int i;
	printf("%d\n",book[i]);
}

void SaiLeitura(){
	printf("Terminei de ler!\n");
}

void EntraEscrita(int dado){
	int i;
	for ( i = 0; i < BOOKSIZE; i++)
		book[i] = dado;
}

void SaiEscrita(){
	printf("Minha obra prima está pronta!\n");
}

void * Leitora ( void * arg ){
	EntraLeitura();
	SaiLeitura();
}

void * Escritora ( void * arg ){
	EntraEscrita();
	SaiEscrita();
}

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
