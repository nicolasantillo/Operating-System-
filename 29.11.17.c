/* 29.11.17
Realizzare un programma multiprocesso in C,
 completo di commento, che svolga  quanto segue: 
 un processo P genera due processi figli P1 e P2. 
 I due figli P1 e P2 eseguono un ciclo indeterminato nel quale generano, 
 ogni secondo, un numero intero casuale compreso tra 0 e 100. 
  Ad ogni estrazione, i numeri generati dai figli sono inviati al processo padre P che provvede a sommarli, 
  a stamparli sullo schermo e a memorizzarli in un file. 
  Il  processo P1 deve gestire  il segnale di interruzione SIGINT. 
  In particolare, all�arrivo di tale segnale P1 deve visualizzare il messaggio di avviso �interruzione disabilitata!�. 
  Il programma viene terminato dal processo padre P quando verifica che la somma dei numeri, che ha ricevuto dai processi figli, 
  assume il valore 100.*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

#define MAX 100
#define LEGGI 0 
#define SCRIVI 1 

int fd_1[2], fd_2[2]; 
pid_t pid1, pid2; 
char *file= "prova.txt"; 
int max=100, min=0; 


void handler(){
	printf("interruzione disabilitata!\n"); 
}

void figlio1() {
  int num1; 
  srand(time(NULL)); 
  signal(SIGINT, handler) ;
  close(fd_1[LEGGI]) ; /*blocco canale di lettura*/
  while (1) {
    num1= rand() % 101;
    write(fd_1[SCRIVI], &num1, sizeof(int)); /*apro canale di scrittura dove inserisco il numero*/
    sleep(1);
  } 

}
void figlio2(){
  int num2; 
  srand(time(NULL)+3); 
  close(fd_2[LEGGI]);  /*blocco canale di lettura*/
  while (1) {
    num2= rand()% 101;     
    write(fd_2[SCRIVI], &num2, sizeof(int)); 
    sleep(1); 
  }
}

void padre (){
  int somma , num1, num2;
  int fd; 
  fd=open(file, O_CREAT|O_WRONLY, 0660); 
  close (fd_1[SCRIVI]) ;
  close (fd_2[SCRIVI]) ;
  do {
    /*apro la pipe in scrittura del figlio 1 e figlio2*/
    read(fd_1[LEGGI] , &num1, sizeof(int)); 
    read(fd_2[LEGGI] , &num2, sizeof(int)); 
    somma=num1+num2; 
    printf("Somma finale %d\n" , somma) ;
    
  }while(somma !=100);  
  
  kill(0, SIGKILL); 

}

int main() {
pid1, pid2;

pid1=fork();
if (pid1==-1 ){
  printf("ERRORE"); 
  exit(0); 

}
else {
  if(pid1==0) { /*codice figlio P1*/
    figlio1();
  }
  else 
    padre(); 
}

pid2=fork(); 
if (pid1==-1 ){
  printf("ERRORE"); 
  exit(0); 

}
else {
  if(pid2==0) { /*codice figlio P2*/
    figlio2();
  }
  else 
    padre(); 
}




}

