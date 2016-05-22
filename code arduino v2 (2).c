#include <Servo.h>

#define pinServo 10
#define capteur A0
#define rayonRoue .....
#define longueurAxe .....
#define servoDesc 120
#define servoMont 60
#define servoStop 90

const byte papD = 2;	//constante pin n°1 de moteur pas à pas de droite
const byte papG = 6;	//constante pin n°1 de moteur pas à pas de gauche
unsigned int mesure;	//variable de stockage de la distance ou de l'angle
char fonction;			//variable de stockage de la fonction choisie (avancer, tourner)
//char BTread;			//variable de stockage de l'information reçue par le Bluetooth
byte pasG = 0;			//variable de stockage du pas actuellement activé de moteur PaP gauche
byte pasD = 0;			//variable de stockage du pas actuellement activé de moteur PaP droit


Servo servo;

void setup() {
	Serial.begin(9600);
	servo.attach(pinServo, 1000, 2000);
	
	for(byte i = papD; i <= papG+3; i++) {
		pinMode(i, OUTPUT);
	}
	
	servo.write(servoMont);						//faire monter le stylo
	while (analogRead(capteur) < 900){}			//attendre que le capteur soit allumé
	servo.write(servoDesc);						//faire descendre le stylo
	while (analogRead(capteur) > 100){}			//attendre que le capteur soit éteint
	servo.write(servoStop);						//stopper le stylo
	
	Serial.print('I'); //indiquer à la télécommande que l'initialisation est terminée
}


void loop() {
    
	if (Serial.available() > 0){			//lorsqu'une info est dispo sur la liaison série
		fonction = Serial.read();				//la premier étant forcement le type de déplacement, on l'enregistre directement
		if (fonction != 'M'){						//si le déplacement n'est pas le changement de position du stylo
			while (Serial.peek() != 'F'){				//tant que ce n'est pas la fin de l'info transmise, signalée par le caractère 'F' 
				if (Serial.available() > 0){				//s'il y a bien qqch à lire
					mesure = mesure*10 + Serial.read() - 48;	//on stocke a la suite la valeur
				}
			}
			Serial.read();		//on effectue une dernière lecture afin d'effacer le 'F'
		}
		
		if (fonction == 'A'){
			avancer(mesure);
		}
		else if (fonction == 'D' || fonction == 'G'){
			tourner(fonction, mesure);
		}
		else if (fonction == 'M'){
			stylo();
		}
	
		Serial.print('F'); //indique à la télécommande 
	}
	
}

//fonction avancer
void avancer(int distance) {

	int nbrPas = 0;
	int nbrPasTotal = distance*(1024/(3.141592*rayonRoue));
	
	while (nbrPas < nbrPasTotal){
    
		pasG = (pasG + 1) % 4;	//augmente d'1 pas le moteur gauche
		pasD = (pasD + 1) % 4;	//augmente d'1 pas le moteur droit
		
		doStep('D', pasD);
		doStep('G', pasG);
		
		nbrPas++;		//indiquer qu'1 pas a été effectué
		delay(20);		//attendre 20ms avance le prochain pas
	}
    
	Serial.print('F');
}

//fonction tourner
void tourner(char sens, int angle) {

	int nbrPas = 0;
	int nbrPasTotal = angle*longueurAxe/(2*rayonRoue*5.625); //angle*longueurAxe*2048/(720*rayonRoue)
	
	while (nbrPas < nbrPasTotal){
		if (sens == 'D'){
			pasD = (pasD + 3) % 4;		//augmente d'1 pas le moteur droit
			pasG = (pasG + 1) % 4;		//diminue d'1 pas le moteur gauche
		}
		else if (sens == 'G'){
			pasD = (pasD + 1) % 4;		//diminue d'1 pas le moteur droit
			pasG = (pasG + 3) % 4;		//augmente d'1 pas le moteur gauche
		}
		doStep('D', pasD);
		doStep('G', pasG);
		
		nbrPas++;		//incrémenté de 1 le nbr de pas effectué
		delay(20);		//attendre 20ms avance le prochain pas
	}
    
}

//fonction modification pas
void doStep(char cote, int pas) {

	int nStep;

    if (cote == 'D'){
        nStep = papD;
    }
    else if(cote == 'G'){
        nStep = papG;
    }
	for (int i=nStep; i <= nStep+3; i++){
		digitalWrite(i, LOW);					//éteinds tout les pas du moteurs
	}
	digitalWrite(nStep + pas, HIGH);			// allume le pas désiré
	
}

//fonction stylo
void stylo(){
	if (analogRead(capteur) < 100){ //si stylo en bas (capteur éteint) alors le monter
        servo.write(servoDesc);
        while (analogRead(capteur) < 100){}
        delay(1000);
        servo.write(servoStop);
    }
	else if (analogRead(capteur) > 900){ //sinon si stylo en haut (capteur allumé) -> le descendre
        servo.write(servoMont);
        while (analogRead(capteur) > 100){}
        servo.write(servoStop);
    }
}