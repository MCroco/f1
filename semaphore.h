#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>

int semid;//variable identitifiant le sémaphore
struct sembuf op;//structure permettant les opérations sur les sémaphores
int nbrPersonne;//variable permettant de connaitre le nombre de lecteur par tour

/*
Cette fonction permet d'initialiser les sémaphores. On passe en argument la clé, 
le nombre d'éléments dans le talbeau de sémaphore et le flags de création.
Il nous retourne l'identifiant du sémaphore
*/
void initSemaphore()
{
	key_t semKey;
	if( (semid = semget(semKey, 2, IPC_CREAT | 0666)) < 0 ) {	
		perror("erreur semget");
		exit(errno);
	}
}

/*
Cette fonction permet de mettre en attente le sémaphore
*/
void wait(int i) {
	op.sem_num = i; 
   	op.sem_op = -1;
	//op.sem_flg = 0;
    	op.sem_flg = SEM_UNDO | IPC_NOWAIT; 
   	semop(semid, &op, 1);	
}

/*
Cette fonction permet d'augmenter la valeur du sémaphore et de réveiller les processus.
*/
void signal(int i) {
	op.sem_num = i;
	op.sem_op = 1;
	op.sem_flg = SEM_UNDO | IPC_NOWAIT; 
   	semop(semid, &op, 1); 
}


/*
Cette fonction permet au lecteur de prendre la main en bloquant par la même occasion 
les rédacteurs. 
*/
void commencerLecture() {
 wait(READ);
 nbrPersonne++;
 if (nbrPersonne == 1) {
	wait(WRITE);
 }
	signal(READ);
}

/*
Une fois la lecture finie, le lecteur redonne la main aux rédacteurs.
*/
void arreterLecture() {
	
 wait(READ);
 nbrPersonne--;
 if (nbrPersonne == 0) {
  signal(WRITE);
 }
 signal(READ);
}

