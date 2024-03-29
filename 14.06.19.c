/*Realizzate un programma multi thread in C che simuli il comportamento di clienti che si recano ad un’agenzia bancaria per svolgere commissioni, in base alle seguenti specifiche:
- i clienti arrivano all’agenzia in istanti di tempo casuale;
- nell’agenzia possono essere presenti al massimo 30 clienti allo stesso tempo;
- un cliente deve attendere fuori se nell’agenzia sono già presenti il massimo numero di clienti;
- quando entra nell’agenzia, un cliente impiega un tempo random, compreso tra TMIN e TMAX, per eseguire le proprie commissioni.
L'applicazione, durante l'esecuzione, deve visualizzare i seguenti messaggi, in accordo con gli stati in cui si può trovare il cliente:
- cliente j arriva all’agenzia
- cliente j attende sulla porta
- cliente j entra nell’agenzia e svolge le commissioni per un tempo T random
- cliente j esce dall’agenzia
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#define TMAX 10
#define TMIN 1
#define NUM_MAX 30  /*numero massimo di clienti presenti nella agenzia*/
#define CLIENTI_NUM 50

pthread_mutex_t m; /*mutex per mutua esclusione*/
pthread_cond_t PIENO;  /*variabile  condizione per verificare se c'è posto o no */

int numero_attuale=0;  
  
/*typedef struct  {
  int cod_fis; 
  pthread_t thread_cliente; 
}Cliente */

void tempo_commissione  ( int t_min ,int t_max) {
  srand(time(NULL)); 
  sleep(rand()%(t_max-t_min+1)+t_min);

}

void *Operazione (void *arg) { /*thread figlio che identifica il cliente*/
  
  int cod_cliente= *( (int*)arg ) ; 
  printf ("Cliente %d arriva all'agenzia \n " , cod_cliente); 
 
  pthread_mutex_lock(&m);    
  if(numero_attuale == NUM_MAX){ /*AGENZIA HA RAGGIUNTO NUMERO MASSIMO*/
    printf ("Cliente %d attende sulla porta \n " , cod_cliente); 
    pthread_cond_wait (&PIENO , &m) ;     

  }
  /*cliente entra in agenzia*/
  numero_attuale= numero_attuale +1 ; 
  printf ("Cliente %d entra in agenzia e svolge le commissioni\n " , cod_cliente); 
  pthread_mutex_unlock(&m);  
  
  tempo_commissione(TMIN,TMAX) ; 

  /*cliente esce dall'agenzia*/
  pthread_mutex_lock(&m);  
  numero_attuale= numero_attuale -1 ; 
  printf ("Cliente %d esce dall'agenzia\n" , cod_cliente); 
  
  pthread_cond_signal(&PIENO) ; 
  pthread_mutex_unlock(&m) ;
  pthread_exit (0);

}
int main () {
  
  pthread_t thread_cliente[CLIENTI_NUM];  /*array di identificatori di thread*/
  pthread_mutex_init(&m, NULL);  
  pthread_cond_init(&PIENO, NULL) ; 
  int i ;   
  for (i=0 ; i<CLIENTI_NUM ; i++){   
      printf ("Creato thread %d \n ",i ); 
      pthread_create(&(thread_cliente[i]), NULL , *Operazione, &i);
     // sleep (3);
  }
  
  for (i=0 ; i<CLIENTI_NUM ; i++){   
      pthread_join(thread_cliente[i], NULL );
  }
 
return 0 ;

}