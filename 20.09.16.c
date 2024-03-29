/*Realizzate un programma multi processo in C, completo di commento, che svolga quanto segue: un processo P
genera due processi figli P1 e P2. Il figlio P1 scrive un messaggio M in un file quindi esegue un ciclo indeterminato
nel quale genera casualmente una sequenza di numeri interi compresi tra 1 e 1000. Quando il numero generato
assume il valore 500 P1 termina il lavoro di generazione dei numeri e invia un segnale a P2. P2, che nel frattempo
non svolgeva alcun lavoro (stato di bloccato), ricevuto il segnale da P1 legge il messaggio M contenuto nel file
salvato da P1 e lo visualizza sullo schermo. */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h>

#define VALGEN 500
#define MAX 1000
#define MIN 1


/* handler per il segnale SIGUSR1 */
void handler (int sig){
	printf("Segnale SIGUSR1 intercettato! \n");
}

int main () {
  srand(time (NULL)); 
  int fd;  /*file descriptor*/
  char file_scrivere[]= "FileScritto.txt"; 
  char mex_in_file[]= "Ciao a tutti";
  
  pid_t ret1 ,ret2 ; 
  ret1 = fork () ; 
  if (ret1==-1) {
    printf ("Errore nella fork \n ");
    exit (0);
  } 
  else
    if (ret1==0) { /*processo figlio1*/
     int n; 
     fd= open (file_scrivere, O_CREAT|O_WRONLY); /*apertura del file_scrivere */
     write (fd , mex_in_file , sizeof(mex_in_file));  /*scrivo nel file il messaggio*/
     close (fd);  /*termino la sessione di accesso al file*/
    
     do {
       n= rand()%(MAX-MIN+1)+MIN ; 
       printf("IL numero generato è %d\n" , n);
     } while (n!=VALGEN) ; 
     /*invio segnale a P2*/
      kill(ret2 , SIGUSR1) ; 
      exit(0);  
    }  
    else {  /*processo padre */

    }
  ret2 = fork () ; 
  if (ret2==-1) {
    printf ("Errore nella fork \n ");
    exit (0);
  } 
  else
    if (ret2==0) { /*processo figlio2*/
     signal(SIGUSR1, handler) ;  
     fd=open (file_scrivere, O_RDONLY); 
     read(fd , mex_in_file, sizeof(mex_in_file)) ; /*P2 legge quello che ha scritto P1*/
     printf("Il messaggio scritto da P1 è %s \n",  mex_in_file);
     close (fd); /*chiudo sessione di accesso al file */

    
    }  
    else {  /*processo padre */
    

    }
      


return 0 ;

}