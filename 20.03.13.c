/*20.02.13
Realizzate un programma multi thread in C, completo di commento, che svolga quanto segue: 
Il thread iniziale che esegue il main crea due thread TH1 e TH2. 
I due thread condividono una variabile intera S. 
TH1, ciclicamente, genera un numero casuale compreso tra 0 e 50 e 
lo sottrae ad S solo nel caso in cui il numero estratto sia minore di S.
 TH2, ciclicamente, genera un numero casuale compreso tra 0 e 100 e lo aggiunge ad S. 
 Un thread termina se il numero estratto ha valore 0. 
 Il programma termina quando il thread main verifica che il valore di S ha superato il
valore di soglia 1000 o i due thread figli hanno terminato la loro esecuzione. 
Prima della terminazione il thread main stampa sullo schermo il valore finale di S e lo salva in un file.*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>


#define SOGLIA 1000 

pthread_mutex_t m; /*mutex per mutua esclusione*/
int s=1; /*buffer condiviso dai due thread*/


void *codice_th1(void *arg) { /*thread 1*/
  int min=0 , max=50 ; 
  int num_1 ;
  srand(time(NULL)); /*seme della random*/
  printf("th1 partito") ;
  do{
    num_1= rand () % (max-min+1)+min; 
    pthread_mutex_lock (&m); /*prologo sezione critica*/
    
    if (num_1<s){
      s=s-num_1;     
    }
    //sleep(1) ; 
    printf("Numero generato da th1= %d\n",num_1) ; 
    pthread_mutex_unlock(&m) ; /*epilogo sezione critica */
  }while (num_1 !=0) ; 
  
   
  printf("Thread 1 terminato ");  
  pthread_exit ((int*)1); 
   
}

void *codice_th2 (void *arg) { /*thread 2*/
  int min= 0 , max = 100; 
  int num_2 ;
  srand(time(NULL)); 
  do {
    pthread_mutex_lock(&m); /*prologo sezione critica*/
    num_2= rand()% (max-min+1)+ min ;
    s=s+num_2 ;
    //sleep(1) ; 
    printf ("Numero generato da th2 =%d\n" , num_2);  
    pthread_mutex_unlock(&m) ; /*epilogo sezione critica*/

  }while (num_2 !=0);
     pthread_exit ((int *)1); 


}

int main () {

  pthread_t th1, th2 ; /*identificatori dei thread 1 e 2*/
  pthread_mutex_init (&m,NULL) ;
  printf("main partito\n") ;

  if(pthread_create (&th1, NULL, *codice_th1, "th1") !=0){
    printf("Errore"); 
    exit(1);
  }

  if(pthread_create (&th2, NULL, *codice_th2, "th2") !=0){
    printf("Errore");    
    exit(1);
  }

  printf("Programma terminato \n");
  
  
  sleep(10);

//join

  return 0 ;
}