#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

int book;
pthread_mutex_t mutex1;
FILE *pont_arq;
int leituras, escritas;

void EntraLeitura(int id){ 

	FILE *arq_temp;
	char *arq_name;

	fprintf(pont_arq, "%s %d %s", "Leit ", id, " entrou\n"); 
	fprintf(pont_arq, "%s %d %s %d %s", "Leit ", id, " leu ", book, "\n");

	arq_temp = calloc(sizeof(FILE*),1);
	arq_name = calloc(sizeof(char),10);
	
	sprintf(arq_name,"%d.txt",id);
	arq_temp = fopen(arq_name,"a+");
	fprintf(arq_temp,"%d\n",book);
	fclose(arq_temp);
///	leituras--;
}

void SaiLeitura(int id ){
	fprintf(pont_arq, "%s %d %s", "Leit ", id, " saiu\n");

}

void EntraEscrita(int id){
	 
	fprintf(pont_arq, "%s %d %s", "Esc ", id , " entrou\n"); fprintf(pont_arq, "%s %d %s %d %s", "Esc ", id, " leu ", book, "\n");
	book = id;
///	escritas--;
	fprintf(pont_arq, "%s %d %s %d %s", "Esc ", id, " escreveu ", book, "\n");
	
}
void SaiEscrita(int id ){ 
	fprintf(pont_arq, "%s %d %s", "Esc ", id, " saiu\n");
	
}

void * Leitora ( void * arg ){
 
	int *id = (int *) arg; 
	while(1) {
		pthread_mutex_lock(&mutex1);
		if(leituras <= 0){
			pthread_mutex_unlock(&mutex1);
			break;
		} else { 
			EntraLeitura(*id); 
			leituras--;
			SaiLeitura(*id); 
			pthread_mutex_unlock(&mutex1);
		}
	}	
	pthread_exit(NULL);
}

void * Escritora ( void * arg ){
	int *id = (int *) arg; 
	while(1){

		pthread_mutex_lock(&mutex1);
		if (escritas <= 0){
			pthread_mutex_unlock(&mutex1);
			break;
		}
		else{
			EntraEscrita(*id); 
			escritas--;
			SaiEscrita(*id);
			pthread_mutex_unlock(&mutex1);
		}
	}
	pthread_exit(NULL);
}

int main (int argc, char *argv[]) { 
	int i, *t; 

	pont_arq = fopen(argv[1], "w");
	int quant_leit = atoi( argv[2]);
	int quant_escr = atoi(argv[3]);
	leituras = atoi(argv[4]);
	escritas = atoi(argv[5]);

	pthread_t threads[quant_escr + quant_leit]; 
	pthread_mutex_init(&mutex1, NULL);

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
