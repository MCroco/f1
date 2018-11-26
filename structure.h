typedef struct voiture{
        int id;//numéro de la voiture
        int secteur1;//temps du secteur 1 tour actuel
        int secteur2;//temps du secteur 2 tour actuel
        int secteur3;//temps du secteur 3 tour actuel
        int tot;//temps tot depuis le debut de la course
        int bestTourPerso; //Meilleur temps réalisé par la voiture lors d'un tour
        int numTour;//numéro du tour actuel
        int pitStop; //nombre de pit stop réalisé par la voiture
        int finis;//1 si la voiture a finis la course
        int out;//1 si il est elimine
};
struct voiture *shm_data;
int shmid = 0;


#define READ 0 //Constante pour lancer le READ
#define WRITE 1 //Constante pour lancer le WRITE

int nbVoiture; //Nombre de voitures surtout pour les qualifs 2 et 3
int hour; //Nombre d'heure, modifiée par la foncion timeConverter pour l'affichage du temps dans le tableau
int min; //Nombre de minutes, modifiée par la foncion timeConverter pour l'affichage du temps dans le tableau
int sec; //Nombre de secondes, modifiée par la foncion timeConverter pour l'affichage du temps dans le tableau
int millisec; //Nombre de millisecondes, modifiée par la foncion timeConverter pour l'affichage du temps dans le tableau
int tempsSecteur; //temps du secteur
int tempsMinSec = 20000; //Temps minimum pour un secteur
int tempsMaxSec = 40000; //Temps maximum pour un secteur
int bestSecteur1 = 40000; //Le meilleur temps réalisé pour le secteur 1
int bestSecteur2 = 40000; //Le meilleur temps réalisé pour le secteur 2
int bestSecteur3 = 75000; //Le meilleur temps réalisé pour le secteur 3
int bestTour = 155000; //Le meilleur temps réalisé pour le tour
int pitStop; //Nombre de pit stop minimum
char out = 'X'; //Pour affciher dans le tableau si une voiture est out

