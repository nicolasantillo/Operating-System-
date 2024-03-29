/*TESTO ESAME:
Realizzate un programma multi thread in C che simuli il comportamento di ciclisti in una stazione di bike sharing (biciclette condivise), 
in base alle seguenti specifiche:
- i ciclisti arrivano alla stazione in istanti di tempo casuale;
- nella stazione possono essere presenti al massimo 50 biciclette allo stesso tempo;
- un ciclista deve attendere se nella stazione non ci sono biciclette;
- quando è disponibile una bici un ciclista la preleva e la usa per un tempo random;
- dopo aver usato la bicicletta il ciclista la riporta alla stazione;
L'applicazione, durante l'esecuzione, deve visualizzare i seguenti messaggi, in accordo con gli stati in cui si può trovare un ciclista:
- ciclista j arriva alla stazione;
- ciclista j preleva una bici;
- ciclista j usa la bici per un tempo T;
- ciclista j riconsegna la bici;
- ciclista j attende una bici.*/



/*ERRORI: 
1. Scordato arg come parametro da passare alla funzione stazione
2. La variabile condivisa andava fuori la funzione che la utilizzava, perchè deve essere visibile globalmente
3. Nella condizione if andava messo >= e non == , così hai bloccato solamente la 50esima bicicletta lasciando
   prelevare dalla 51esima in poi
4. Stazione sarebbe ciclista
5. Ciclisti arrivano in tempo random --> Randsleep nella for che crea i thread
6. Il confronto con NUM_BICI nel thread doveva essere con numero_bici non con id*/


#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#define NUM_BICI 50 
#define NUM_MAX_BICI 100
#define T_MIN 0
#define T_MAX 10


pthread_mutex_t m  ;  /*mutex per mutua esclusione*/
pthread_cond_t PIENO ; /* variabile condizione*/

int numero_bici= NUM_BICI ; /*variabile condivisa*/

void uso_bici  (int t_min , int t_max){ /*tempo in cui il cliente usa la bici*/
  srand(time(NULL)) ;  /*seme della random*/
  
  sleep(rand() % (t_max-t_min+1)+t_min);   

return ;
  
}

int randSleep(int min, int max){
    srand(time(NULL)); 
    return (rand()%(max-min) + min);
}



void *Stazione (void *arg) {
  int id = * ((int *)arg );  
   
  printf ("cliente %d arriva alla stazione\n", id );
  
  pthread_mutex_lock (&m) ; 
  if (numero_bici== 0) { 
    printf ("cliente %d attende in coda la bici\n", id );
    pthread_cond_wait(&PIENO,  &m) ;  
   
  }  
  numero_bici= numero_bici -1 ; 
  printf ("cliente %d  preleva la bici, con numero di bici %d \n", id , numero_bici  ); 
 
  pthread_mutex_unlock(&m);  

  printf("cliente %d utilizza la bici \n",  id ); 
  uso_bici(T_MIN , T_MAX);  

  pthread_mutex_lock (&m) ; 
  numero_bici= numero_bici +1 ; 
  printf("cliente %d consegna  la bici\n" ,  id) ; 
  

  pthread_cond_signal(&PIENO);  
  pthread_mutex_unlock(&m);  


  pthread_exit (0) ;  


}

int main (){
  
  pthread_t th[NUM_MAX_BICI];  /*array di tipo thread di dimensione num_max_bici*/
  pthread_mutex_init (&m, NULL) ; 
  pthread_cond_init (&PIENO, NULL) ; 

  int i ;  

  /*creazione dei thread*/
  for(i=0 ;  i<NUM_MAX_BICI ; i++){
    randSleep(1,3); 
    pthread_create(&th[i] , NULL , *Stazione , &i);     
    
  } 
  
  /*attesa terminazione dei thread*/
  for(i=0 ;  i<NUM_MAX_BICI ; i++){
    pthread_join(th[i] , NULL);     
  } 
  

return 0 ; 
}