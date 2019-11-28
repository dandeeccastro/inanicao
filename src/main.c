#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#define NTHREADS 4
#define BOOKSIZE 100
int book;
pthread_mutex_t mutex1;

FILE *pont_arq;

void EntraLeitura(int id){ 
	fprintf(pont_arq, "%s %d %s", "Leit ", id, " entrou\n"); //leu
	fprintf(pont_arq, "%s %d %s %d %s", "Leit ", id, " leu ", book, "\n");
}

void SaiLeitura(int id ){
	fprintf(pont_arq, "%s %d %s", "Leit ", id, " saiu\n");
}

void EntraEscrita(int id){

	fprintf(pont_arq, "%s %d %s", "Esc ", id , " entrou\n"); fprintf(pont_arq, "%s %d %s %d %s", "Esc ", id, " leu ", book, "\n");
	book = id;
	fprintf(pont_arq, "%s %d %s %d %s", "Esc ", id, " escreveu ", book, "\n");
}
void SaiEscrita(int id ){ 
	fprintf(pont_arq, "%s %d %s", "Esc ", id, " saiu\n");
}

void * Leitora ( void * arg ){
	pthread_mutex_lock(&mutex1); 
	int *id = (int *) arg; 
	EntraLeitura(*id); 
	SaiLeitura(*id); 
	pthread_mutex_unlock(&mutex1);
	pthread_exit(NULL);
}
void * Escritora ( void * arg ){
	pthread_mutex_lock(&mutex1); 
	int *id = (int *) arg; 
	EntraEscrita(*id); 
	SaiEscrita(*id);
	pthread_mutex_unlock(&mutex1);
	pthread_exit(NULL);
}

int main (int argc, char *argv[]) { 
	int i, *t; pthread_t threads[NTHREADS]; 
	pont_arq = fopen(argv[1], "w");
	pthread_mutex_init(&mutex1, NULL);
	printf("Inicio da main\n");
	for (i = 0; i < NTHREADS; i++){ t = malloc(sizeof(int)); *t = i; if (i%2) { pthread_create( &threads[i], NULL, Leitora, (void *) t); } else { pthread_create( &threads[i], NULL, Escritora , (void *) t); } }
	printf("Main gerou todas as threads\n");
	for (i = 0; i < NTHREADS; i++) pthread_join(threads[i],NULL);
	return 0;}
