#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <string.h>

int num_leituras, num_escritas, quant_leitoras, quant_escritoras;
int var_global;
int leitoras_global = 0, escritoras_global = 0;
FILE * log_file;
sem_t mutex_leit, mutex_escr, mutex_geral, lendo, escrevendo;

typedef struct {
	int id;
	char type[5];
} thread_log;

/**
 * wait(mutex_leit); l++;
 * if (l == 1) wait(escr)
 * post(mutex_leit)
 *
 * Isso faz com que o primeiro cara confira se a condição
 * pra ele acessar a global é valida
 */

/**
 * wait(mutex_leit); l--;
 * if (l == 0) post(escr)
 * post(mutex_leit)
 *
 * Isso faz com que o ultimo cara avise para a outra especie
 * que não tem mais nenhum deles lá
 */
void * Leitora( void * arg ){
	int * id = (int *) arg;
	while (1) {
		sem_wait(&mutex_leit);
		if (num_leituras <= 0){
			sem_post(&mutex_leit);
			break;
		} else {
			sem_post(&mutex_leit);

			sem_wait(&lendo);
			sem_wait(&mutex_leit);
			leitoras_global++;
			if (leitoras_global == 1)
				sem_wait(&escrevendo);
			sem_post(&mutex_leit);
			sem_post(&lendo);

			EntraLeitura(*id);
			SaiLeitura(*id);

			sem_wait(&mutex_leit);
			num_leituras--;
			leitoras_global--;
			if (leitoras_global == 0)
				sem_post(&escrevendo);
			sem_post(&mutex_leit);
		}
	}
	pthread_exit(NULL);
}

void EntraLeitura(int id){
	FILE *leit_file;
	char *leit_filename;

	sem_wait(&mutex_geral);
	fprintf(log_file, "Leit %d entrou\n", id);
	fprintf(log_file, "Leit %d leu %d\n", id, var_global);
	sem_post(&mutex_geral);

	leit_file = calloc(sizeof(FILE*),1);
	leit_filename = calloc(sizeof(char),20);

	sprintf(leit_filename,"%d.txt",id);
	leit_file = fopen(leit_filename, "a+");
	fprintf(leit_file,"%d\n",var_global);
	fclose(leit_file);

	free(leit_filename);
}

void SaiLeitura(int id){
	sem_wait(&mutex_geral);
	fprintf(log_file, "Leit %d saiu\n", id);
	sem_post(&mutex_geral);
}

void * Escritora ( void * arg ){
	int * id = (int *) arg;
	while (1) {
		sem_wait(&mutex_escr);
		if (num_escritas <= 0){
			sem_post(&mutex_escr);
			break;
		} else {
			sem_post(&mutex_escr);

			sem_wait(&mutex_escr);
			escritoras_global++;
			if (escritoras_global == 1)
				sem_wait(&lendo);
			sem_post(&mutex_escr);

			sem_wait(&escrevendo);
			EntraEscrita(*id);
			num_escritas--;
			SaiEscrita(*id);
			sem_post(&escrevendo);

			sem_wait(&mutex_escr);
			escritoras_global--;
			if (escritoras_global == 0)
				sem_post(&lendo);
			sem_post(&mutex_escr);
		}
	}
	pthread_exit(NULL);
}

void EntraEscrita(int id){
	sem_wait(&mutex_geral);
	fprintf(log_file, "Esc %d entrou\n", id);
	fprintf(log_file, "Esc %d leu %d\n", id, var_global);

	var_global = id;

	fprintf(log_file, "Esc %d escreveu %d\n", id, var_global);	
	sem_post(&mutex_geral);
}

void SaiEscrita(int id){
	sem_wait(&mutex_geral);
	fprintf(log_file, "Esc %d saiu\n", id);
	sem_post(&mutex_geral);
}

int main( int argc, char * argv[]){

	if (argc < 6) {
		printf("Uso: ./main.out <quant_leitoras> <quant_escritoras> <num_leituras> <num_escritas> <nome_arquivo_log>\n");
		return -1;
	}

	quant_leitoras = atoi(argv[1]);
	quant_escritoras = atoi(argv[2]);
	num_leituras = atoi(argv[3]);
	num_escritas = atoi(argv[4]);
	log_file = fopen(argv[5],"w+");

	sem_init(&mutex_escr,0,1);
	sem_init(&mutex_leit,0,1);
	sem_init(&mutex_geral,0,1);
	sem_init(&lendo,0,quant_leitoras);
	sem_init(&escrevendo,0,1);

	int i, * t;
	pthread_t threads[quant_leitoras + quant_escritoras];

	for (i = 0; i < quant_leitoras + quant_escritoras; i++){
		t = malloc(sizeof(int));
		*t = i;
		if ( i < quant_leitoras)
			pthread_create( &threads[i], NULL, Leitora, (void *) t);
		else 
			pthread_create( &threads[i], NULL, Escritora, (void *) t);
	}

	for (i = 0; i < quant_leitoras + quant_escritoras; i++)
		pthread_join(threads[i], NULL);

	return 0;
}
