#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

int book;
FILE *pont_arq;
int leituras, escritas;
sem_t gen_mutex, escr_mutex, leit_mutex;

void EntraLeitura(int id){ 

	// Variáveis da função
	FILE *arq_temp;
	char *arq_name;

	sem_wait(&gen_mutex);
	// Lendo o valor da variável 
	fprintf(pont_arq, "Leit %d entrou\n", id); 
	fprintf(pont_arq, "Leit %d leu %d\n", id, book);

	// Callocando espaço para as variáveis 
	arq_temp = calloc(sizeof(FILE*),1);
	arq_name = calloc(sizeof(char),10);
	
	// Abro o arquivo com o nome correto e escrevo o 
	// conteúdo da variável 
	sprintf(arq_name,"%d.txt",id);
	arq_temp = fopen(arq_name,"a+");
	fprintf(arq_temp,"%d\n",book);
	fclose(arq_temp);
}

void SaiLeitura(int id ){
	fprintf(pont_arq, "Leit %d saiu\n",id);
	sem_post(&gen_mutex);
}

void EntraEscrita(int id){
	sem_wait(&gen_mutex);
	// Entra na variável, le e escreve o seu id
	fprintf(pont_arq, "Esc %d entrou\n", id); 
	fprintf(pont_arq, "Esc %d leu %d\n", id, book);
	book = id;
	fprintf(pont_arq, "Esc %d escreveu %d\n", id, book);
	
}
void SaiEscrita(int id ){ 
	fprintf(pont_arq, "Esc %d saiu\n",id);
	sem_post(&gen_mutex);
}

void * Leitora ( void * arg ){
	int *id = (int *) arg; 
	while(1) {
		sem_wait(&leit_mutex);
		// Se ainda não acabaram as leituras, dou unlock e saio do loop 
		if(leituras <= 0){
			sem_post(&leit_mutex);
			break;
		// Senão, leio
		} else { 
			EntraLeitura(*id); 
			leituras--;
			SaiLeitura(*id); 
			sem_post(&leit_mutex);
		}
	}	
	pthread_exit(NULL);
}

void * Escritora ( void * arg ){
	int *id = (int *) arg; 
	while(1){
		sem_wait(&escr_mutex);
		if (escritas <= 0){
			sem_post(&escr_mutex);
			break;
		}
		else{
			EntraEscrita(*id); 
			escritas--;
			SaiEscrita(*id);
			sem_post(&escr_mutex);
		}
	}
	pthread_exit(NULL);
}

int main (int argc, char *argv[]) { 
	int i, *t; 

	pont_arq = fopen(argv[1], "w+");
	int quant_leit = atoi( argv[2]);
	int quant_escr = atoi(argv[3]);
	leituras = atoi(argv[4]);
	escritas = atoi(argv[5]);

	sem_init(&gen_mutex,0,1);
	sem_init(&escr_mutex,0,1);
	sem_init(&leit_mutex,0,1);

	pthread_t threads[quant_escr + quant_leit]; 

	printf("Inicio da main\n");

	for (i = 0; i < quant_escr + quant_leit; i++){ 
		t = malloc(sizeof(int)); 
		*t = i; 

		if(i < quant_leit)
			pthread_create( &threads[i], NULL, Leitora, (void *) t); 
		else
		  pthread_create( &threads[i], NULL, Escritora , (void *) t); 

	}

	printf("Main gerou todas as threads\n");
	for (i = 0; i < quant_leit + quant_escr; i++) pthread_join(threads[i],NULL);
	return 0;}
