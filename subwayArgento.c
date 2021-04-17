#include <stdio.h>      // libreria estandar
#include <stdlib.h>     // para usar exit y funciones de la libreria standard
#include <string.h>
#include <pthread.h>    // para usar threads
#include <semaphore.h>  // para usar semaforos
#include <unistd.h>		//para hacer sleep

#define LIMITE 50

int posicion=1;
sem_t sem_salarMutex;
sem_t sem_cocinarMutex;
sem_t sem_hornearMutex;
sem_t sem_ganador;

struct semaforos {
    sem_t sem_cortar;
    sem_t sem_mezclar;
	sem_t sem_salar;
	sem_t sem_empanar;
	sem_t sem_cocinar;
	sem_t sem_hornear;
	sem_t sem_preparandoVerduras;
};

struct paso {
    char accion [LIMITE];
    char ingredientes[4][LIMITE];
};

struct parametro {
    int equipo_param;
    struct semaforos semaforos_param;
    struct paso pasos_param[8];
};

//funcion para imprimir las acciones y los ingredientes de la accion
void* imprimirAccion(void *data, char *accionIn) {
	struct parametro *mydata = data;
	//calculo la longitud del array de pasos 
	int sizeArray = (int)( sizeof(mydata->pasos_param) / sizeof(mydata->pasos_param[0]));
	FILE* fichero = fopen("archivo.txt", "at");
	int i;
	for(i = 0; i < sizeArray; i ++){
		//pregunto si la accion del array es igual a la pasada por parametro (si es igual la funcion strcmp devuelve cero)
		if(strcmp(mydata->pasos_param[i].accion, accionIn) == 0){
			fprintf(fichero,"\tEquipo %d : %s \n",mydata->equipo_param,mydata->pasos_param[i].ingredientes[0]);
			printf("\tEquipo %d : %s \n",mydata->equipo_param,mydata->pasos_param[i].ingredientes[0]);
		}
	}
	fclose(fichero);
}

void* imprimirFInalAccion(void *data, char *accionIn) {
	struct parametro *mydata = data;
	int sizeArray = (int)( sizeof(mydata->pasos_param) / sizeof(mydata->pasos_param[0]));
	FILE* fichero = fopen("archivo.txt", "at");
	int i;
	for(i = 0; i < sizeArray; i ++){
		if(strcmp(mydata->pasos_param[i].accion, accionIn) == 0){
			fprintf(fichero,"\tEquipo %d : %s \n",mydata->equipo_param,mydata->pasos_param[i].ingredientes[1]);
			printf("\tEquipo %d : %s \n",mydata->equipo_param,mydata->pasos_param[i].ingredientes[1]);
		}
	}
	fclose(fichero);
}

void* preparandoVerduras(void *data) {
	char *accion = "preparando verduras";
	struct parametro *mydata = data;

    sem_wait(&mydata->semaforos_param.sem_preparandoVerduras);
		imprimirAccion(mydata,accion);
		usleep( 5000000 );
		imprimirFInalAccion(mydata,accion);

    pthread_exit(NULL);
}

void* hornear(void *data) {
	char *accion = "hornear";
	struct parametro *mydata = data;

	sem_wait(&sem_hornearMutex);
		imprimirAccion(mydata,accion);
		usleep( 6000000 );
		imprimirFInalAccion(mydata,accion);
	sem_post(&sem_hornearMutex);
    
    pthread_exit(NULL);
}

void* cocinar(void *data) {
	char *accion = "cocinar"; 
	struct parametro *mydata = data;

    sem_wait(&mydata->semaforos_param.sem_cocinar);
	sem_wait(&sem_cocinarMutex);
		imprimirAccion(mydata,accion);
		usleep( 9000000 );
		imprimirFInalAccion(mydata,accion);
	sem_post(&sem_cocinarMutex);

    pthread_exit(NULL);
}

void* empanar(void *data) {
	char *accion = "empanar";
	struct parametro *mydata = data;

    sem_wait(&mydata->semaforos_param.sem_empanar);
		imprimirAccion(mydata,accion);
		usleep( 5000000 );
		imprimirFInalAccion(mydata,accion);
	sem_post(&mydata->semaforos_param.sem_cocinar);

    pthread_exit(NULL);
}

void* salar(void *data) {
	char *accion = "salar"; 
	struct parametro *mydata = data;

    sem_wait(&mydata->semaforos_param.sem_salar);
	sem_wait(&sem_salarMutex);
		imprimirAccion(mydata,accion);
		usleep( 2000000 );
		imprimirFInalAccion(mydata,accion);
	sem_post(&sem_salarMutex);
    sem_post(&mydata->semaforos_param.sem_empanar);
    
    pthread_exit(NULL);
}

void* mezclar(void *data) {
	char *accion = "mezclar"; 
	struct parametro *mydata = data;

    sem_wait(&mydata->semaforos_param.sem_mezclar);
		imprimirAccion(mydata,accion);
		usleep( 5000000 );
		imprimirFInalAccion(mydata,accion);
	sem_post(&mydata->semaforos_param.sem_salar);
    
    pthread_exit(NULL);
}

void* cortar(void *data) {
	//creo el nombre de la accion de la funcion 
	char *accion = "cortar";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;
	//semaforo para entrar a la seccion critica
    sem_wait(&mydata->semaforos_param.sem_cortar);
		//llamo a la funcion imprimir le paso el struct y la accion de la funcion
		imprimirAccion(mydata,accion);
  		//uso sleep para simular que que pasa tiempo
		usleep( 5000000 );
		//llamo a la funcion imprimirFinalAccion para imprimir el final de la accion
		imprimirFInalAccion(mydata,accion);
		//doy la señal a la siguiente accion (cortar me habilita mezclar)
    sem_post(&mydata->semaforos_param.sem_mezclar);
    
    pthread_exit(NULL);
}

void* ejecutarReceta(void *i) {

	//variables semaforos
    sem_t sem_cortar;
    sem_t sem_mezclar;
	sem_t sem_salar;
	sem_t sem_empanar;
	sem_t sem_cocinar;
	sem_t sem_preparandoVerduras;
	
	//variables hilos
	pthread_t p1; 
	pthread_t p2; 
	pthread_t p3;
	pthread_t p4; 
	pthread_t p5; 
	pthread_t p6;
	pthread_t p7; 
	
	//numero del equipo (casteo el puntero a un int)
	int p = *((int *) i);
	
	printf("Ejecutando equipo %d \n", p);

	//reservo memoria para el struct
	struct parametro *pthread_data = malloc(sizeof(struct parametro));
	
	//seteo numero de grupo
	pthread_data->equipo_param = p;
	
	//seteo semaforos
    pthread_data->semaforos_param.sem_cortar = sem_cortar;
	pthread_data->semaforos_param.sem_mezclar = sem_mezclar;
	pthread_data->semaforos_param.sem_salar = sem_salar;
	pthread_data->semaforos_param.sem_empanar = sem_empanar;
	pthread_data->semaforos_param.sem_cocinar = sem_cocinar;
	pthread_data->semaforos_param.sem_preparandoVerduras = sem_preparandoVerduras;


	//******* algoritmo de lectura de receta sin terminar********
	FILE *archi;
	archi=fopen("receta.txt","r");
	char cadena[100];
	int h=0;
	int j=0;
	while(fgets(cadena,100,archi)!=NULL){
		char *token=strtok(cadena,";");
		//strcpy(pthread_data->pasos_param[k].ingredientes[0],token);
		while(token!=NULL){
		//	strcpy(pthread_data->pasos_param[k].ingredientes[1],token);
		token=strtok(NULL,";");
			j++;
		}
		h++;
	}

	//seteo las acciones y los ingredientes
    strcpy(pthread_data->pasos_param[0].accion, "cortar");
	strcpy(pthread_data->pasos_param[0].ingredientes[0], "cortando ajo y perejil");
	strcpy(pthread_data->pasos_param[0].ingredientes[1], "ajo y perejil cortados");

	strcpy(pthread_data->pasos_param[1].accion, "mezclar");
	strcpy(pthread_data->pasos_param[1].ingredientes[0], "mezclando ajo y perjil con el huevo y la carne");
	strcpy(pthread_data->pasos_param[1].ingredientes[1], "preparacion mezclada");
    
    strcpy(pthread_data->pasos_param[2].accion, "salar");
	strcpy(pthread_data->pasos_param[2].ingredientes[0], "salando preparacion");
	strcpy(pthread_data->pasos_param[2].ingredientes[1], "preparacion salada");

    strcpy(pthread_data->pasos_param[3].accion, "empanar");
	strcpy(pthread_data->pasos_param[3].ingredientes[0], "empanando la carne con pan rallado");
	strcpy(pthread_data->pasos_param[3].ingredientes[1], "milanesa empanada");
    
    strcpy(pthread_data->pasos_param[4].accion, "cocinar");
	strcpy(pthread_data->pasos_param[4].ingredientes[0], "cocinando milanesa");
	strcpy(pthread_data->pasos_param[4].ingredientes[1], "milanesa cocinada");
	
	strcpy(pthread_data->pasos_param[5].accion, "hornear");
	strcpy(pthread_data->pasos_param[5].ingredientes[0], "horneando 1 pan");
	strcpy(pthread_data->pasos_param[5].ingredientes[1], "Fin de horneado, retiro 1 pan");

	strcpy(pthread_data->pasos_param[6].accion, "preparando verduras");
	strcpy(pthread_data->pasos_param[6].ingredientes[0], "preparando lechuga, tomate, cebolla y pepino");
	strcpy(pthread_data->pasos_param[6].ingredientes[1], "verduras preparadas");

	
	//inicializo los semaforos
    sem_init(&(pthread_data->semaforos_param.sem_cortar),0,1);
	sem_init(&(pthread_data->semaforos_param.sem_mezclar),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_salar),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_empanar),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_cocinar),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_preparandoVerduras),0,1);

	//creo los hilos a todos les paso el struct creado (el mismo a todos los hilos) ya que todos comparten los semaforos 
    int rc;

    rc = pthread_create(&p1,                           //identificador unico
                        NULL,                          //atributos del thread
                        cortar,     			        //funcion a ejecutar
                        pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia
	rc = pthread_create(&p2,NULL,mezclar,pthread_data);
	rc = pthread_create(&p3,NULL,salar,pthread_data);
	rc = pthread_create(&p4,NULL,empanar,pthread_data);
	rc = pthread_create(&p5,NULL,cocinar,pthread_data);
	rc = pthread_create(&p6,NULL,hornear,pthread_data);
	rc = pthread_create(&p7,NULL,preparandoVerduras,pthread_data);
	
	//join de todos los hilos
	pthread_join (p1,NULL);
	pthread_join (p2,NULL);
	pthread_join (p3,NULL);
	pthread_join (p4,NULL);
	pthread_join (p5,NULL);
	pthread_join (p6,NULL);
	pthread_join (p7,NULL);	
	
	//abro archivo para escritura
	FILE* fichero = fopen("archivo.txt", "at");
	fprintf(fichero,"\tEquipo %d : Armando sandwich\n",p);
	printf("\tEquipo %d : Armando sandwich\n",p);

	//uso sleep para simular que que pasa tiempo
	usleep( 3000000 );
	//defino el ganador 
	sem_wait(&sem_ganador);
		if (posicion==1){	
			fprintf(fichero,"\tSandwich Listo! el equipo %d es el ganador!! \n",p);
			printf("\tSandwich Listo! el equipo %d es el ganador!! \n",p);
		}else{
			fprintf(fichero,"\tSandwich Listo! el equipo %d ha terminado en posicion numero %d \n",p,posicion);
			printf("\tSandwich Listo! el equipo %d ha terminado en posicion numero %d \n",p,posicion);
		}
		posicion++;
	sem_post(&sem_ganador);

	fclose(fichero);

	//valido que el hilo se alla creado bien 
    if (rc){
        printf("Error:unable to create thread, %d \n", rc);
        exit(-1);
    }
	
	//destruccion de los semaforos 
    sem_destroy(&sem_cortar);
	sem_destroy(&sem_mezclar);
	sem_destroy(&sem_salar);
	sem_destroy(&sem_empanar);
	sem_destroy(&sem_cocinar);
	sem_destroy(&sem_preparandoVerduras);
	
	//salida del hilo
	pthread_exit(NULL);
}

int main (){
//creo/limpio un archivo nuevo para escribir la ejecucion
FILE* fi = fopen("archivo.txt", "wt");
fclose(fi);

//creo los nombres de los equipos 
int rc;
int *equipoNombre1 =malloc(sizeof(*equipoNombre1));
int *equipoNombre2 =malloc(sizeof(*equipoNombre2));
int *equipoNombre3 =malloc(sizeof(*equipoNombre3));
int *equipoNombre4 =malloc(sizeof(*equipoNombre4));

*equipoNombre1 = 1;
*equipoNombre2 = 2;
*equipoNombre3 = 3;
*equipoNombre4 = 4;

//creo las variables los hilos de los equipos
pthread_t equipo1; 
pthread_t equipo2;
pthread_t equipo3;
pthread_t equipo4;

sem_init(&sem_salarMutex,0,1);
sem_init(&sem_cocinarMutex,0,1);
sem_init(&sem_hornearMutex,0,2);
sem_init(&sem_ganador,0,1);

//inicializo los hilos de los equipos
rc = pthread_create(&equipo1,NULL,ejecutarReceta,equipoNombre1); 
rc = pthread_create(&equipo2,NULL,ejecutarReceta,equipoNombre2);
rc = pthread_create(&equipo3,NULL,ejecutarReceta,equipoNombre3); 
rc = pthread_create(&equipo4,NULL,ejecutarReceta,equipoNombre4); 

if (rc){
    printf("Error:unable to create thread, %d \n", rc);
    exit(-1);
} 

pthread_join (equipo1,NULL);
pthread_join (equipo2,NULL);
pthread_join (equipo3,NULL);
pthread_join (equipo4,NULL);

sem_destroy(&sem_salarMutex);
sem_destroy(&sem_cocinarMutex);
sem_destroy(&sem_hornearMutex);
sem_destroy(&sem_ganador);


pthread_exit(NULL);
}


//Para compilar:   gcc subwayArgento.c -o ejecutable -lpthread
//Para ejecutar:   ./ejecutable
