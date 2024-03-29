/*22.02.12
Realizzate un programma multi-thread in C, completo di commento, che svolga quanto segue:
Il thread main crea due thread figli T1 e T2. 
Entrambi i thread figli eseguono un ciclo indeterminato durante il
quale, ad ogni iterazione, generano un numero intero casuale compreso tra 0 e 5 che comunicano al padre. Il thread
padre, per ogni coppia di numeri che riceve dai thread figli ne fa la somma e nel caso essa sia un numero pari
incrementa di 1 la variabile S1, nel caso in cui la somma sia un numero dispari incrementa di 1 la variabile S2; se la
somma vale 0 assegna sia ad S1 che a S2 il valore 0. Quanto il thread padre verifica che il valore di S1 o di S2 ha
superato il valore 11, visualizza sullo schermo il valore delle due variabili e il programma termina. La sequenza
temporale delle operazioni eseguite dai thread deve essere: 
1) T1 estrae un numero e lo comunica al thread padre;
2) T2 estrae un numero e lo comunica al thread padre; 
3) il thread padre esegue le operazioni sopra descritte; e così
via.
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

/*SEQUENZA: 
turno=1 parte il thread 1, turno=2 parte thread 2 , turno=0 parte il thread padre
*/
pthread_mutex_t m;  /*mutex per mutua esclusione*/
int num_1 , num_2;  /*buffer in cui scrivono i thread*/
int turno=1;  /*variabile identifico condizione relativa al thread che può operare*/
int max=5, min=0;
pthread_cond_t cond_turno; 

void *codice_th1 (void *arg){

  srand(time(NULL)); 
  while (1) {
    pthread_mutex_lock(&m);
    if (turno!=1) 
      pthread_cond_wait(&cond_turno, &m);
    
    num_1 = rand()% (max-min+1) +min; 
    sleep(1) ;

    turno=2;

    printf("Numero generato da th1= %d\n",num_1) ; 
    pthread_cond_signal(&cond_turno); 
    pthread_mutex_unlock(&m);
  
  }
}

void *codice_th2(void *arg){
  srand(time(NULL)); 
  while (1) {
    pthread_mutex_lock(&m);
    if(turno!=2)
      pthread_cond_wait(&cond_turno, &m);
    
    num_2 = rand() % (max-min+1)+ min; 
    sleep(1); 
    printf("Numero generato da th2= %d\n",num_2) ; 

    turno=0;

    pthread_cond_signal(&cond_turno); 
    pthread_mutex_unlock(&m);
  
  }
}

int main () {
  int fine=0; 
  int s1=0, s2=0, somma ;
  pthread_t th1, th2; /*identificatori dei thread*/ 
  pthread_mutex_init(&m, NULL); 
  pthread_cond_init(&cond_turno,  NULL) ;
  
  if(pthread_create (&th1, NULL, *codice_th1, "th1") !=0){
    printf("Errore"); 
    exit(1);
  }
  if(pthread_create (&th2, NULL, *codice_th2, "th2") !=0){
    printf("Errore"); 
    exit(1);
  }
  do{
    pthread_mutex_lock(&m) ; 
    if(turno!=0)
      pthread_cond_wait(&cond_turno, &m);
    
    printf("num1=%d, num2=%d\n" , num_1, num_2);

    somma=num_1+num_2;
    if(somma==0){
      s1=0;
      s2=0;
    } 
    else {
      if (somma%2 !=0) 
        s2=s2+1;      
      else 
        s1=s1+1;
    }
    
    turno=1;

    pthread_cond_signal(&cond_turno); 
    pthread_mutex_unlock(&m) ; 
    
    if(s1 > 11||s2 > 11)
      fine=1; 
  }while (!fine); 
  
  printf("Programma terminato con s1= %d , s2=%d\n" , s1,s2);

  return 0 ;

}