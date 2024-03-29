 /*14.09.2017
Realizzate un programma multi thread in C, completo di commento, che svolga quanto segue: 
il thread iniziale che esegue il main crea due thread TH1 e TH2. 
I due thread condividono due variabili intere S1 e S2 sulle quali operano in mutua esclusione. 
TH1 genera periodicamente, ogni secondo, un numero casuale compreso tra 1 e 50 e
lo sottrae a S1 se il numero estratto è dispari, mentre lo sottrae a S2 se il numero estratto è pari. 
TH2 genera periodicamente, ogni due secondi, 
un numero casuale compreso tra 1 e 100 e lo aggiunge ad S1 ed incrementa di 1
il valore di S2. 
Il programma termina quando TH1 verifica che il valore della somma S1+S2 ha superato il valore
1000. Prima della terminazione del programma il thread main stampa su schermo il valore finale della somma
S1+S2.
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define MAXTH1 50; 
#define MAXTH2 100; 

pthread_mutex_t  m; /* mutex per mutua esclusione */
int s1=0, s2=0;  /* variabili globali su cui operano th1,th2 */

void *codice_th1(void *arg) {
  int gen_num; 
  
  while(s1+s2<=1000){
    pthread_mutex_lock(&m); /*prologo*/
    gen_num= rand() % MAXTH1 + 1 ;
     printf("numero generato da th1 %d\n",  gen_num);
    if(gen_num %2)      /*condizione dispari*/
      s1= s1-gen_num; 
    else                /*condizione pari*/
      s2= s2-gen_num; 
    pthread_mutex_unlock(&m); /*epilogo*/
    sleep(1);
  }
  pthread_exit(0) ;
    
}

void *codice_th2 (void *arg){
  int gen_num; 
  
  while(s1+s2<=1000){
    pthread_mutex_lock(&m); /*prologo*/
    gen_num= rand() % MAXTH2 + 1;
    printf("numero generato da th2 %d\n",  gen_num);
    s1= s1+gen_num; 
    s2++; 
    pthread_mutex_unlock(&m); /*epilogo*/
    sleep(2);
  }
  pthread_exit(0) ;
}

int main(){
  pthread_t th1, th2; 
  int var_attesa; 
  srand(time(NULL)); /*genera il seme iniziale*/
  pthread_mutex_init(&m , NULL);
  // creazione e attivazione del primo thread
  if (pthread_create(&th1,NULL,codice_th1, "th1")!=0){
    fprintf(stderr,"Errore di creazione thread 1 \n");
    exit(1);
  } 
   // creazione e attivazione del secondo thread
  if (pthread_create(&th2,NULL,codice_th2, "th2")!=0){
    fprintf(stderr,"Errore di creazione thread 2 \n");
    exit(1);
  }
  /*attesa terminazione primo thread*/
  var_attesa= pthread_join(th1,NULL);
  if (var_attesa != 0) 
    printf ("join fallito %d \n" , var_attesa);  
  else 
    printf ("Il thread 1 è terminato \n");    
  

 /*attesa terminazione secondo thread*/
  var_attesa= pthread_join(th2,NULL);
  if (var_attesa != 0) 
    printf ("join fallito %d \n" , var_attesa);  
  else 
    printf ("Il thread 2 è terminato \n");    
  
  printf("La somma finale di s1 e s2 è = %d \n" , s1+s2); 
  return 0;

}