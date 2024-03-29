/*Realizzate un programma multi-thread con modello master-worker in C, che svolga quanto segue:
Il thread main (master) crea 20 nuovi thread (worker) ad intervalli di tempo compresi tra 1 e 3 secondi e quindi si
sospende. 
I worker creati possono essere di due tipi: Tipo1 e Tipo2. La scelta del tipo è casuale. Un worker di Tipo1
aggiunge, ogni secondo, ad una variabile TOT condivisa, un valore casuale compreso tra 1 e 100. Un worker di
Tipo2, invece, sottrae, ogni secondo, al valore di TOT un valore casuale compreso tra 1 e 50. Se un worker estrae il
numero 17 si blocca e può essere riattivato solo da un altro thread del suo stesso tipo. L’applicazione termina
quando un thread estrae il valore 13. Prima di terminare il thread main stampa sullo schermo il valore di TOT.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>

#define MAXWORKER 20

int tot=0; /*variabile condivisa*/

pthread_mutex_t m  ; /*mutex per la mutua esclusione*/
pthread_cond_t trovato17_w1;
pthread_cond_t trovato17_w2; 
pid_t processo;  


/* handler per il segnale SIGUSR1 */
void handler (int sig){
	printf("Segnale SIGUSR1 intercettato! \n");
    printf ("Il valore di tot è %d\n ", tot) ; 

    exit(0); 
}


void randSleep (int t_min , int t_max) {
  int ns ; 
  srand(time(NULL));
  ns= rand() % (t_max-t_min+1) +t_min;  
  sleep(ns);

  return ;
}

void *Worker1 (void *arg){
  int t_min=1, t_max=100; 
  int num1; 
  srand(time(NULL));
  do{
    pthread_mutex_lock(&m); 
  
    num1= rand()%(t_max-t_min+1)+t_min; 
    
    if (num1==17){
      pthread_cond_wait (&trovato17_w1, &m); 
          
    }
    tot= tot+num1 ;
    printf("Totale modificato da W1 %d con numero %d\n" , tot, num1);
    pthread_cond_signal(&trovato17_w1);
    pthread_mutex_unlock(&m); 
    sleep(1); 
  }while (num1 != 13);
  
  //processo= getpid();
  kill( 0,SIGUSR1);
  
  pthread_exit(0); 

}

void *Worker2(void *arg){
  int t_min=1, t_max=50; 
  int num1; 
  srand(time(NULL));
  do{
    pthread_mutex_lock(&m); 
  
    num1= rand()%(t_max-t_min+1)+t_min; 
    
    if (num1==17){
      pthread_cond_wait (&trovato17_w2, &m); 
          
    }
    tot= tot-num1 ;
    printf("Totale modificato da W2 %d con numero %d\n" , tot, num1);
    pthread_cond_signal(&trovato17_w2);
    pthread_mutex_unlock(&m); 
    sleep(1); 
  }while (num1 != 13);
  
 // processo= getpid();
  kill( 0,SIGUSR1);
  
  pthread_exit(0); 

}

int main (){
  pthread_t w1,  w2 ; 
  
  int i,  n_random;
  srand(time(NULL)); 
  pthread_mutex_init(&m , NULL); 
  pthread_cond_init(&trovato17_w1 , NULL);
  pthread_cond_init(&trovato17_w2 , NULL);

  signal (SIGUSR1 , handler); 
  
  for (i=0 ; i<MAXWORKER ; i++){   
    n_random= rand()%2; 
    printf("Numero generato main %d\n", n_random);
    if (n_random==0)
      pthread_create(&w1, NULL , *Worker1, "w1");
    
    else 
      pthread_create(&w2, NULL , *Worker2, "w2");
    
   randSleep(1,3); 
  }
  
  pause(); /*Si mette in attesa di una signal */

  return 0;

}
