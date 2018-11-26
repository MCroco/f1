#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <structure.h>
#include <utils.h>
#include <semaphore.h>

void shm_write(int id);
void genTemps(void);

int nbTour;//nombre de tours 
struct voiture participant;//struct de la voiture du process

int main(int argc, char *argv[]){//argv[1]=id de la voiture  argv[2]=n° du proces  argv[3]=nbr  pit  argv[4]== nbr tour
	srand(time(NULL) ^ (getpid()<<20)); //pour que tous les fork aient un autreseed de random
	shm_init();//initialisation de la shm
	//initSemaphore();
	nbTour = atoi(argv[4]);//attribution de l'argv[4] a nbTour
	participant.id=atoi(argv[1]);//conversion et atribution de argv[1] a l'id de la struct participant;
	pitStop = atoi(argv[3]);//nbr min de pit a faire
	participant.pitStop=0;//nbr pit min + random un nombre de pitStop et l'affect dans la struct
	participant.numTour=0;
	participant.tot=0;
	participant.bestTourPerso = 155000;//initialisation du bestTourPerso a la plus grande valeur possible
	int tour=1;
	int pitChoix;
	while(participant.numTour<nbTour){//boucle tant que la voiture a pas finis la course
		genTemps();//generation du temps pour le tour
		shm_write(atoi(argv[2]));//appele la fonction pour ecrire dans la shm
		sleep(2.5);//attente aproximative de l'afficheur
	}
	exit(0);
}
/*fonction ecrivant dans la shm la struct en local
*id place dans la shm où le process doi ecrire
*/
void shm_write(int id){
        wait(WRITE);
        if(participant.numTour==nbTour||participant.out==1){//si la voiture a finis tt les tour ou est out 
                participant.finis=1;//dire a la voiture qu'elle a finis sont nombre de tour
        }   
        shm_data[id]=participant;//ecriture dans la shm
        signal(WRITE);
}
/*fonction générant le temps de chaque secteur pour le tour actuel
*/
void genTemps(){
	int cpt;
	int out = randomise(0, 1000);// randomise des chances pour que la voiture soit out
	if(participant.numTour == 1){
		participant.tot = 0;
		//Ligne pour les qualifs
		//participant[cpt].secteur3 = 155000;
	}
			
	if (out < 1 && participant.out == 0){//si le participant n'est pas encore out et que out est inférieur a 1
					     //la voiture est out
		participant.out = 1;//voiture est out
	}else{	
		participant.numTour+=1;//tour suivant
		if(participant.out ==0){//si le participant est en course
			participant.secteur1=randomise(tempsMinSec,tempsMaxSec);//randomise du temps secteur1
		}else{//si le participant est out
			participant.secteur1=40000;//le secteur 1 est au plus lent
		}
		if(participant.out ==0){//si le participant est en course
			participant.secteur2=participant.secteur1+randomise(tempsMinSec,tempsMaxSec);//randomise du temps
												     //secteur 2
		}else{//si le participant est out
			participant.secteur2 = 80000;//le secteur 2 est au plus lent
		}
		tempsSecteur = participant.secteur2-participant.secteur1;
		int nombre;// % de chance de faire un pitStop
		if(participant.numTour == 1 || participant.numTour == nbTour-1){//si la voiture est au premier ou
										//au dernier tour
			nombre = 40;//les chance de pitStop est a 40
		}else{//sinon
			nombre = randomise(1,100);//randomise des chance de pitStop
		}
		int tempsPit = 0;
		if(nombre < participant.numTour+5 && participant.pitStop < pitStop){//il doit faire un pit stop
			tempsPit = randomise(25000, 35000);//random du temps pit
			participant.pitStop++;//augmentation du nombre de pit fait
		}
                if(nombre < 5 && participant.pitStop >= pitStop && participant.pitStop <= pitStop+2){
                        tempsPit = randomise(25000, 35000);
                        participant.pitStop++;
                }

		if(participant.out == 0){//si le participant est en course
			participant.secteur3 = participant.secteur2+randomise(tempsMinSec,tempsMaxSec) + tempsPit;
				//randomise du temps secteur3 +temps de eventuel pit
		}else{//si participant est out
			participant.secteur3 = 155000;//secteur 3 est au plus lent
		}
		if(participant.secteur3 < participant.bestTourPerso) {
			participant.bestTourPerso = participant.secteur3;
		}
		participant.tot = participant.tot + participant.secteur3;

		//cette ligne est pour la course finale
		tempsSecteur = participant.secteur3-participant.secteur2;
		if(participant.secteur3 < bestTour){//si le tour act est plus rapide que le best tour
			bestTour = participant.secteur3;//best tour est le temps du tour act
		}
	}
}