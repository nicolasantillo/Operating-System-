/*Realizzate un programma multi thread in C, completo di commento, che svolga quanto segue:
Il thread main crea due thread figli T1 e T2. 
Entrambi i thread figli eseguono un ciclo indeterminato durante il
quale, ad ogni iterazione, 
generano un numero intero casuale compreso tra 0 e 1000 
che comunicano al padre. 
Il thread padre, per ogni coppia di numeri che riceve dai thread figli ne fa la somma e la visualizza. 
Il programma deve terminare quando i due numeri estratti da T1 e T2 sono uguali o la somma dei due numeri supera il valore
1800. 
La sequenza temporale delle operazioni eseguite dai thread deve essere: 
1) T1 estrae un numero e lo comunica al thread padre; 
2) T2 estrae un numero e lo comunica al thread padre; 
3) il thread padre fa la somma dei numeri ricevuti ed eventualmente termina il programma; e così via.*/


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <stdbool.h>


#define MIN 0; 
#define MAX 1000; 

pthread_mutex_t m ;  /*mutex per la mutua esclusione*/
sem_t sem_prod1, sem_prod2; /*semafori produttori th1 e th2*/
sem_t sem_cons1, sem_cons2; /*semaforo consumatore */
int s1 =-1, s2=-2 ; /*buffer dove th1 e th2 producono numeri randm*/
 
void *codice_th (void *arg) { /*produttore (di numeri random)*/
  int id= (int *)arg ;
  int gen_num; 
  srand(time(NULL)+id); /*seme della rand*/
  printf ("Partito thread %d \n" , id) ;
  
  while(1){
    pthread_mutex_lock(&m); 
    gen_num= rand() % 1001;  /*produco nuovo messaggio (MAX-MIN+1)+MIN*/
    if( id==1) {
      sem_wait(&sem_prod1) ;     
      s1=gen_num ; /*inserisco messaggio nel buffer s1*/
      printf("Valore s1 = %d del thread th1\n", s1);
      sem_post(&sem_cons1) ; 
    }
    else {
      sem_wait(&sem_prod2) ;     
      s2=gen_num ; /*inserisco messaggio nel buffer s2*/
      printf("Valore s2 = %d del thread th2\n", s2);
      sem_post(&sem_cons2) ; 

    }
    pthread_mutex_unlock(&m) ;  
  }

}


int main() {
  int msg1, msg2; 
  bool fine= 0; 
  pthread_t th1 ,th2;
  pthread_mutex_init(&m, NULL) ;
  sem_init(&sem_prod1,0, 1); 
  sem_init(&sem_cons1,0, 0); 
  sem_init(&sem_prod2,0, 1); 
  sem_init(&sem_cons2,0, 0); 
  if(pthread_create(&th1,NULL, *codice_th, (int) 1) !=0){
    printf("Errore")  ;  
    exit(1) ; 
  } 
  if(pthread_create(&th2,NULL, *codice_th, (int) 2) !=0){
    printf("Errore") ;   
    exit(1) ; 
  } 

  do {
    pthread_mutex_lock(&m); 
    sem_wait(&sem_cons1); 
    msg1=s1; /*prelievo messaggio dal buffer s1*/
    sem_post(&sem_prod1); 
    pthread_mutex_unlock(&m); 
    pthread_mutex_lock(&m); 
    sem_wait(&sem_cons2); 
    msg2=s2; /*prelievo messaggio dal buffer s2*/
    sem_post(&sem_prod2); 
    pthread_mutex_unlock(&m); 
    printf("Stampa s1=%d e s2=%d\n", msg1 , msg2); 
    
    if(msg1== msg2 || msg1+msg2 >1800 ){
      fine= 1; 
      printf("Fine con somma %d\n" , msg1+msg2); 

    }

  }while (!fine);
  
  return 0; 

}