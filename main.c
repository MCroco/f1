#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structure.h"
#include "utils.h"
#include "semaphore.h"


void shm_read(void);
void forking(int pitMin,int nbTour);

pid_t pids[20];//pid des processus fils
struct voiture participant [20];//structure de voiture en local copie de la shm


int main(int argc, char *argv[]){//argv[1]== nombre de pit nim argv[2]== nbr de tour  argv[3]==mode/etat 0-1  //argv[4]== nbr Voiture 

	//srand(time(NULL) ^ (getpid()<<20));
	if(atoi(argv[3])!=1 && atoi(argv[3])!=0){//si le argv[3] est different de 0/1 exit 1
		printf("mauvais arguments");
		exit(1);
	}
	if(argv[4]!= NULL){//si il y a un argv[4] on attribue la valeur a nbVoiture
		nbVoiture=atoi(argv[4]);
	}else{//si pas de argv[4] on attribue un valeur par default
		nbVoiture=20;
	}
	shm_init();// initialisation de la shm
	initSemaphore();
	if(argv[1] != NULL && argv[2]!= NULL){// si argv[1] et argv[2] est une valeur
		forking(atoi(argv[1]),atoi(argv[2]));
	}else{//si pas de argv[1] et de argv[2] on utilise des valeurs par default
		forking(2,30);//si pas d'argv[1] et argv[2] alors parametre par default
	}
	if(pids[0]>1){//processus père
		int tour;
		sleep(2);// attente de l'initialisation des voitures
		while(finish()==0){//boucle tant que toutes les voitures n'ont pas finis
			shm_read();//lecture dans la shm

			trierTab(participant,atoi(argv[3]));//triage du tableau de voitures en
							    //fonction de argv[3]
			actTable(participant,1,atoi(argv[3]));//affichage du tableau pour le 1er secteur
			sleep(1);

			actTable(participant,2,atoi(argv[3]));//affichage du tableau pour le 2eme secteur
			sleep(1);

			actTable(participant,3,atoi(argv[3]));//affichage du tableau pour le 3eme secteur
			sleep(2);

			bestSecteur1 = 40000;//remise a la plus grosse valeur de temps des secteurs
			bestSecteur2 = 40000;
			bestSecteur3 = 75000;
		}
	//fermeture de la shm
	shmdt ((const struct voiture *)shm_data);
	shmctl(shmid,IPC_RMID, NULL);
	}
	return 0;
}

void shm_read(){//fonction lisant les données de la shm
        commencerLecture();
        memcpy(participant,shm_data,sizeof(struct voiture)*nbVoiture);
        arreterLecture();
}

/*fonction créant les processus fils
 *pitMin  nombre de pit nimimum a faire dans la course
 *nbTour  nombre de tour que dure la course 
*/
void forking(int pitMin, int nbTour){
	int cpt;
	char voiture[20][3]={"44","77","3","33","5","7","31","19","2","55","8","20","30","9","94","27","11","18","10","14"};
		//tableau des numéros des voitures
	char pit[2];
	char tour[3];
	sprintf(tour, "%d",nbTour);//conversion de nbTour en char
	sprintf(pit, "%d", pitMin);//conversion de pitMin en char
	for(cpt=0;cpt<nbVoiture;cpt++){
		char idFils[2];
		sprintf(idFils, "%d", cpt);//conversion de cpt en char
		char *const arguments[]= {"./voiture",voiture[cpt],idFils,pit,tour,NULL};//arguments passé au programme voiture
		if((pids[cpt] = fork())<0){//création des processus fils
			perror("fork");
			abort();
		}else if(pids[cpt]==0){
			if(execv("./voiture",arguments) == -1){//execution du programme voiture avec les paramètres
				printf("erreur pids: %d\n",cpt);
			}
			exit(0);
		}
	}
}

/*fonction verifiant si toutes les voitures n'ont pas finis la course
 * retourn 1 si toutes les voitures ont finis la course
 * retourn 0 si il reste des voitures en course
*/
int finish(){
	int i,cpt=0;
	for(i=0; i<15;i++){
		if(participant[i].finis==1){
			cpt+=1;
		}
	}
	if(cpt==15) return 1;
	else return 0;
	
}
//-------------------------afficheur-------------------
