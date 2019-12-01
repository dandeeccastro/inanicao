#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

int num_leituras, num_escritas, quant_leitoras, quant_escritoras;
int var_global;
FILE * log_file;

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
		if (num_leituras <= 0){
			break;
		} else {


			EntraLeitura(*id);
			SaiLeitura(*id);

			num_leituras--;
		}
	}
	pthread_exit(NULL);
}

void EntraLeitura(int id){
	FILE *leit_file;
	char *leit_filename;

	fprintf(log_file, "Leit %d entrou\n", id);
	fprintf(log_file, "Leit %d leu %d\n", id, var_global);

	leit_file = calloc(sizeof(FILE*),1);
	leit_filename = calloc(sizeof(char),20);

	sprintf(leit_filename,"%d.txt",id);
	leit_file = fopen(leit_filename, "a+");
	fprintf(leit_file,"%d\n",var_global);
	fclose(leit_file);

	free(leit_filename);
}

void SaiLeitura(int id){
	fprintf(log_file, "Leit %d saiu\n", id);
}

void * Escritora ( void * arg ){
	int * id = (int *) arg;
	while (1) {
		if (num_escritas <= 0){
			break;
		} else {


			EntraEscrita(*id);
			num_escritas--;
			SaiEscrita(*id);

		}
	}
	pthread_exit(NULL);
}

void EntraEscrita(int id){
	fprintf(log_file, "Esc %d entrou\n", id);
	fprintf(log_file, "Esc %d leu %d\n", id, var_global);

	var_global = id;

	fprintf(log_file, "Esc %d escreveu %d\n", id, var_global);	
}

void SaiEscrita(int id){
	fprintf(log_file, "Esc %d saiu\n", id);
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
