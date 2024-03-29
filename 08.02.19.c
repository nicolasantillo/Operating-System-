/*Realizzate un programma multi thread in C che simuli il comportamento di automobili che entrano in un
autolavaggio, in base alle seguenti specifiche:
- le auto arrivano all’autolavaggio in istanti di tempo random;
- nell’autolavaggio sono disponibili 7 postazioni di lavaggio;
- un’auto deve attendere in coda se tutte le postazioni sono occupate;
- quando un’auto accede a una postazione, può occuparla fino a un tempo Tmax;
- dimensionate la simulazione per 100 auto.
L'applicazione, durante l'esecuzione, deve visualizzare i seguenti messaggi, in accordo con gli stati in cui si può
trovare un'auto:
- auto j arriva all’autolavaggio;
- auto j attende che una postazione si liberi;
- auto j entra in una postazione;
- auto j esce dall’autolavaggio. */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#define MAX_AUTO 100
#define POSTAZIONI 7

pthread_mutex_t m; /*mutex per mutua esclusione*/
pthread_cond_t PIENO ; /*variabile condizione per gestire quando autolavaggio pieno*/

int posti=0 ; 

void tempo_lavaggio( int t_min ,int t_max) {
  srand(time(NULL)); 
  sleep(rand()%(t_max-t_min+1)+t_min);

}

void *Autolavaggio (void *arg) {
  int automobile;
  
  automobile= * ( int *)arg ; /*inserisco nella variabile auto, il numero dell'auto entrata*/
  printf ("L'auto %d arriva nell'autolavaggio\n" ,automobile); 

  pthread_mutex_lock(&m); 
  if (posti>=POSTAZIONI ) { /*caso di postazioni tutte occupate */
    printf("L'auto %d attende che una postazione si liberi \n" , automobile);  
    pthread_cond_wait(&PIENO, &m); 

  }
  posti++; 
  printf ("Auto %d entra in una postazione\n ", automobile) ;
  pthread_mutex_unlock(&m); 
  tempo_lavaggio (1,5); 

  /*finito il lavaggio*/
  pthread_mutex_lock(&m); 
  posti--;  
  printf("Auto %d esce dall'autolavaggio\n" , automobile);

  pthread_cond_signal (&PIENO);  
  pthread_mutex_unlock(&m); 

  pthread_exit (0) ; 

}

int  main () {
  pthread_t thread_auto[MAX_AUTO] ; 
  pthread_mutex_init(&m, NULL);  
  pthread_cond_init(&PIENO, NULL) ; 
  
  int i ;   
  for (i=0 ; i<MAX_AUTO ; i++){   
      printf ("Creato thread %d \n ",i ); 
      pthread_create(&(thread_auto[i]), NULL , *Autolavaggio, &i);
     // sleep (3);
  }
  
  for (i=0 ; i<MAX_AUTO ; i++){   
    pthread_join(thread_auto[i], NULL );
  }
 

 return 0 ; 


}