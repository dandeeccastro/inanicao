#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

int num_leituras, num_escritas, quant_leitoras, quant_escritoras;
int var_global;
FILE * log_file;
pthread_mutex_t mutex;
pthread_cond_t cond_leit, cond_escr;
int escr_fila = 0, leit_fila = 0, lendo = 0, escrevendo = 0;

typedef struct {
	int id;
	char type[5];
} thread_log;

int vez;

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

void EntraLeitura(int id){

	int var_local;
	pthread_mutex_lock(&mutex);
	if(escrevendo > 0) {leit_fila++;
	//while(escrevendo > 0) { //verficando escritoras na fila
		pthread_cond_wait(&cond_leit, &mutex);}
	//}
	
	FILE *leit_file;
	char *leit_filename;

	fprintf(log_file, "Leit %d entrou\n", id);
	fprintf(log_file, "Leit %d leu %d\n", id, var_global);

	var_local = var_global;
	pthread_mutex_unlock(&mutex);
	
	leit_file = calloc(sizeof(FILE*),1);
	leit_filename = calloc(sizeof(char),20);

	sprintf(leit_filename,"%d.txt",id);
	leit_file = fopen(leit_filename, "a+");
	fprintf(leit_file,"%d\n",var_local);
	fclose(leit_file);

	free(leit_filename);
}
//escr, ler
void SaiLeitura(int id){
	pthread_mutex_lock(&mutex);
	lendo--;
	if (lendo == 0 ) {
		escrevendo = escr_fila;
		escr_fila = 0;
		pthread_cond_signal(&cond_escr);
	}
	fprintf(log_file, "Leit %d saiu\n", id);
	pthread_mutex_unlock(&mutex);
}

void EntraEscrita(int id){
	pthread_mutex_lock(&mutex);
	if(lendo > 0) {escr_fila++;
	//while(lendo > 0){
		pthread_cond_wait(&cond_escr, &mutex);}
	//}

    fprintf(log_file, "Esc %d entrou\n", id);
	fprintf(log_file, "Esc %d leu %d\n", id, var_global);

	var_global = id;

	fprintf(log_file, "Esc %d escreveu %d\n", id, var_global);	

	
}

void SaiEscrita(int id){

	escrevendo--;
	if(escrevendo == 0 ) {
		pthread_cond_broadcast(&cond_leit);
		lendo = leit_fila;
		leit_fila = 0;
	}	
	else pthread_cond_signal(&cond_leit);
	fprintf(log_file, "Esc %d saiu\n", id);
	pthread_mutex_unlock(&mutex);
}

void * Escritora ( void * arg ){
	int * id = (int *) arg;
	while (1) {
		pthread_mutex_lock(&mutex);
		num_escritas--;
		if (num_escritas < 0){
			pthread_mutex_unlock(&mutex);
			break;
		} else {
			pthread_mutex_unlock(&mutex);
			EntraEscrita(*id);
			SaiEscrita(*id);

		}
	}
	pthread_exit(NULL);
}

void * Leitora( void * arg ){
	int * id = (int *) arg;
	while (1) {
		pthread_mutex_lock(&mutex);
		num_leituras--;
		if (num_leituras < 0){
			pthread_mutex_unlock(&mutex);
			break;
		} else {
			pthread_mutex_unlock(&mutex);
			EntraLeitura(*id);
			SaiLeitura(*id);
		}
	}
	pthread_exit(NULL);
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

	pthread_cond_init(&cond_escr, NULL);
	pthread_cond_init(&cond_leit, NULL);

	pthread_mutex_init(&mutex, NULL);

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
