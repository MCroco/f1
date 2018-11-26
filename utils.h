//Fonction pour initialiser la mémoire partagée
void shm_init(){
        key_t key = 5678;
        shmid = shmget(key,nbVoiture*sizeof(struct voiture),IPC_CREAT|0666);
        shm_data = (struct voiture *) shmat(shmid, NULL, 0); 
        if(shm_data == (const struct voiture *) -1){
                perror("shmat");
        }   
    
}
//Fonction pour gérnérer des nombres randoms entre un minimum et un maximum
int randomise(int min, int max){
	return rand()%(max-min+1)+min;
}
//Fonction pour convertir le temps généré en millisecondes en heures, minutes, secondes, millisecondes
void timeConverter(int b){
     	hour = b/3600000;
    	min = (b%3600000)/60000;
	sec = (b%60000)/1000;
    	millisec = b-hour*3600000-min*60000-sec*1000;
}
/*
* Fonction pour trier le tableau des voitures 
* soit en fonction des meilleurs temps au tour 
* soit en fonction du nombre de tour et de leur temps total
*/
void trierTab(struct voiture participant[], int tri){//tri deffini sur quoi trier
	int i = 0;
	for(i = 0; i < nbVoiture; i++){
		int j = 1;
		for(j=1; j < nbVoiture-i; j++){
			switch(tri){
				case 0:if(participant[i].numTour < participant[i+j].numTour){
						struct voiture stocka;
								struct voiture stockb;
								stocka = participant[i];
								stockb = participant[i+j];

								participant[i] = stockb;
								participant[i+j] = stocka;
					}else if(participant[i].numTour == participant[i+j].numTour){
							if(participant[i].tot > participant[i+j].tot){
								struct voiture stocka;
								struct voiture stockb;
								stocka = participant[i];
								stockb = participant[i+j];

								participant[i] = stockb;
								participant[i+j] = stocka;
							}
						}
				break;
				case 1:	if(participant[i].bestTourPerso > participant[i+j].bestTourPerso){ 
                                                struct voiture stocka;
                                                struct voiture stockb; 
                                                stocka = participant[i];
                                                stockb = participant[i+j];

                                                participant[i] = stockb;
                                                participant[i+j] = stocka;
                                        }
				break;

			}
		}
	}
}
/*
* Fonction d'affichage du classement selon 3 arguments
* La structure de voiture des participant
* L'affichage auquel on est, c'est à dire affcihage secteur 1, 2 ou tout le tableau
* Le mode dans lequel on est, essai/qualif ou course
*/
void actTable(struct voiture participant[], int tour, int mode){
	int cpt;
	printf("nbr voiture\ttour\tSecteur 1\t\tSecteur2\t\tSecteur3\t\ttemps tour\t\t");
	if(mode==1){
		printf("Best Tour Perso");
	}else{
		printf("Temps Tot");
	}
	printf("\t\tPitStop\n\n");
	for(cpt=0;cpt<nbVoiture;cpt++){
		if(participant[cpt].out ==0){
			//-------------------------------------------secteur1--------------------------	
			if(tour >=1){
				timeConverter(participant[cpt].secteur1);
				printf("%d\t\t",participant[cpt].id);
				printf("%d\t",participant[cpt].numTour);
				printf("%ds%dms  \t\t",sec, millisec);
				if(participant[cpt].secteur1 < bestSecteur1){
					bestSecteur1 = participant[cpt].secteur1;
				}
			}
			//-------------------------------------------secteur2---------------------------
			if(tour >= 2){
				timeConverter((participant[cpt].secteur2-participant[cpt].secteur1));
				printf("%ds%dms  \t\t",sec,millisec);
                                if(participant[cpt].secteur2-participant[cpt].secteur1 < bestSecteur2){
                                        bestSecteur2 = participant[cpt].secteur2-participant[cpt].secteur1;
                                }

			}
			//-------------------------------------------secteur3--------------------------
			if(tour>=3){
				timeConverter((participant[cpt].secteur3-participant[cpt].secteur2));
				printf("%dmn%ds%dms  \t\t",min, sec, millisec);
				timeConverter(participant[cpt].secteur3);
				printf("%dmn%ds%dms  \t\t",min, sec, millisec);
				if(mode==1){
					timeConverter(participant[cpt].bestTourPerso);
                                        printf("%dmn%ds%dms  \t\t",min, sec, millisec);

				}else{
					timeConverter(participant[cpt].tot);
					printf("%dmn%ds%dms  \t\t",min, sec, millisec);
				}
				printf("%d",participant[cpt].pitStop);
				participant[cpt].tot+=participant[cpt].secteur3;
                                if(participant[cpt].secteur3-participant[cpt].secteur2 < bestSecteur3){
                                        bestSecteur3 = participant[cpt].secteur3-participant[cpt].secteur2;
                                }
				if(participant[cpt].secteur3<bestTour){
					bestTour = participant[cpt].secteur3;
				}

			}
		printf("\n");
		}
	}
	for(cpt=0;cpt<nbVoiture;cpt++){
		if(participant[cpt].out != 0){
			printf("%d\t\t",participant[cpt].id);
			printf("%c\t\t",out);
			printf("%c\t\t\t",out);
			printf("%c\t\t\t",out);
			printf("%c\t\t\t",out);
			printf("%c\t\t\t",out);
			printf("%c\n",out);
		}


	}

	if(tour >=1){
		timeConverter(bestSecteur1);
		printf("\t\tBest S1: %dmn%ds%dms  \t\t",min, sec, millisec);
	}
	if(tour	>=2){
		timeConverter(bestSecteur2);
                printf("Best S2: %dmn%ds%dms  \t",min, sec, millisec);
	}	
	if(tour>=3){
		timeConverter(bestSecteur3);
                printf("Best S3: %dmn%ds%dms \t",min, sec, millisec);
		
		timeConverter(bestTour);
                printf("Best Tour: %dmn%ds%dms ",min, sec, millisec);

	}
	printf("\n\n\n\n");


}
