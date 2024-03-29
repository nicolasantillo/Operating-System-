/*Realizzate un programma multi-processo in C, completo di commento, che svolge quanto segue:
Il processo padre P crea una matrice di numeri interi di dimensione NxM assegnando a ciascun elemento della
matrice un valore casuale compreso tra 0 e 1024. Dopo aver creato la matrice, il processo padre crea 2 processi figli
P1 e P2. P1 ha il compito di eseguire la somma di tutte le righe pari della matrice (considerate 0 pari) mentre P2 ha
il compito di eseguire la somma di tutte le righe dispari. Ciascun processo figlio comunica la somma che ha
calcolato al processo padre e quindi passa nello stato di bloccato. Il processo padre, ottenute le somme dai processi
figli, le confronta con il valore di una variabile SOGLIA. Se almeno uno dei valori calcolati dai figli, supera il
valore di SOGLIA, il processo padre invia un segnale a ciascuno dei processi figli che ne provoca la riattivazione. I
figli, così riattivati, visualizzano sullo schermo la somma che hanno calcolato e quindi terminano. Nel caso in cui
le somme calcolate dai processi figli non superino il valore di SOGLIA, il padre semplicemente fa terminare
l’applicazione multi-processo.*/

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

#define M 5
#define N 5
#define LEGGI 0
#define SCRIVI 1

long SOGLIA= N*M*1024/4;
  /* handler per il segnale SIGINT */
void handler (int sig){
  printf("segnale %d\n",sig);
}
int main() {
  int sum_pari, sum_dispari;
  pid_t pid1, pid2;
  int fd_1[2], fd_2[2] ; /*file descriptor della pipe*/
  int i, j ;
  int SIZE=N*M*sizeof(int);
  const char *nome = "MEMCOND";
  int shm_fd; 
  void *shm_ptr ;
  int a[N][M];
  int sum=0;

 if (pipe(fd_1)<0){
    printf ("ERRORE") ;  
    exit (1);
  }
  pid1= fork() ; /*genero il primo processo */
  if (pid1==-1){
    printf ("ERRORE"); 
    exit (0); 
  } 
  else{
    if (pid1==0){ /*codice figlio P1*/
       sum=0;
       signal(SIGUSR1,handler);
       shm_fd= shm_open(nome, O_RDONLY, 0666);
       shm_ptr= mmap (0, SIZE ,PROT_READ,  MAP_SHARED, shm_fd, 0) ;
       memcpy(a, shm_ptr, SIZE); 
       
       for(i=0 ; i<M ; i=i+2) {
         for (j=0 ; j<N ; j++ ){
            sum= sum+a[i][j];
            printf("valore matrice p1 %d\n" , a[i][j]) ;
         }
       }
       close(fd_1[LEGGI]); /*chiudo canale che non uso*/
       write(fd_1[SCRIVI],  &sum , sizeof(int));     
       pause(); 
       printf("somma pari= %d\n", sum); 

    }
    else { /*codice padre*/
      int min=0 , max=1024 ;
      srand(time(NULL));
      shm_fd= shm_open(nome, O_CREAT| O_RDWR, 0666);
      ftruncate(shm_fd, SIZE); 
      shm_ptr= mmap (0, SIZE ,PROT_WRITE, MAP_SHARED, shm_fd, 0) ;
    
      for (i=0 ; i<M ; i++){
        for (j=0 ; j<N ; j++ ){
          a[i][j]= rand() % (max-min+1)+min; 
          printf("valore matrice scritta %d\n" , a[i][j]) ;
        }    
      }    
      memcpy(shm_ptr, a , SIZE );  
      close(fd_1[SCRIVI]); /*chiude canale che non usa*/
      read(fd_1[LEGGI], &sum_pari, sizeof(int)); 
      printf("Somma pari %d\n" , sum_pari); 
    
      if (pipe(fd_2)<0){
        printf ("ERRORE") ;  
        exit (1);
      }
      
      pid2= fork() ; /*genero il secondo processo */
      if (pid2==-1){
        printf ("ERRORE"); 
        exit (0); 
      } 
      else{
        if (pid2==0){ /*codice figlio P2*/
          sum=0;
          signal(SIGUSR1,handler);
          shm_fd= shm_open(nome, O_RDONLY, 0666);
          shm_ptr= mmap (0, SIZE ,PROT_READ,  MAP_SHARED, shm_fd, 0) ;
          memcpy(a, shm_ptr, SIZE); 
       
          for(i=1 ; i<M ; i=i+2) {
            for (j=0 ; j<N ; j++ ){
             sum= sum+a[i][j];
             printf("valore matrice p2 %d\n" , a[i][j]) ;
            }
          }
          close(fd_2[LEGGI]); /*chiudo canale che non uso*/
          write(fd_2[SCRIVI],  &sum , sizeof(int));      
          pause(); 
          printf("somma dispari= %d\n", sum); 

        }
        else { /*codice padre*/ 
          close(fd_2[SCRIVI]); /*chiude canale che non usa*/
          read(fd_2[LEGGI], &sum_dispari, sizeof(int)); 
          if(sum_pari>SOGLIA || sum_dispari >SOGLIA){
           kill(pid1, SIGUSR1); 
           kill(pid2, SIGUSR1);
                  
          }
          else 
            kill(0,SIGKILL) ; 
            
       }
    }

    
    
    }
  }
  
 


  return 0 ;
}
