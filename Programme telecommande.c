void loop(){ // debut de la fonction loop()

	// --- ici instructions à exécuter par le programme principal --- 

	touche = clavier.getKey(); // lecture de la touche appuyée

	if (touche != NO_KEY){ // si une touche a été frappée -- gestion de la touche appuyée 
		
		if(erreur){ //s'il y a eu une erreur précédemment 
			lcd.setCursor(5.1);
			lcd.print("      ");
			lcd.setCursor(pos, 0);
		}
		
		if ((touche == 'A' || touche == 'D' || touche == 'G' || touche == 'M') && fonction == 0){
			//stockage de la fonction à effectuer (s'il n'y a pas une fonction déjà présente)
			fonction = touche;
			switch(touche){
				case 'A':
					lcd.print("Avancer ");
					pos = pos+8;
					break;
				case 'D':
					lcd.print("Droite ");
					pos = pos+7;
					break;
				case 'G':
					lcd.print("Gauche ");
					pos = pos+7;
					break;
				case 'M':
					if (m = 0){
						lcd.print("Monter");
						m = true;
					}
					else if (m = 1){
						lcd.print("Descendre");
						m = false;
					}
					break;
			}	
				
		}
		
		//si on recoit un chiffre et qu'une fonction est bien présente
		else if ((48 <= touche && touche <=57) && fonction != 0 && fonction != 'M'){
			//stock le chiffre a la suite de la mesure pésente
			mesure = mesure*10 + touche-48;
			pos++;
			lcd.print(touche);
			if (mesure > 6500) {
			//prevention contre les mesure >65000 qui cause un roll-over à cause du stockage (car unsigned int)
				mesure = 0;
				fonction = 0;
				pos = 0;
				lcd.clear();
				lcd.print("  Valeur trop        grande");
				delay(2000);
				lcd.clear();
			}
		}
	
		//si on ordonne l'execution 
		else if (touche == 'E' && (mesure != 0 || fonction == 'M'){
			Serial.print(fonction);
			Serial.print(mesure);
			Serial.print('F');
			
			lcd.clear();
			lcd.print("  Execution...");
			while (Serial.available()=0){}
			
			mesure = 0;
			fonction = 0;
			pos = 0;
			//reset des variables apres execution
		}

		else if (touche == 'S'){
			//si bouton supprimer, reset des variables
			fonction = 0;
			mesure = 0;
			pos = 0;
			lcd.clear();
		}
		
		else {
			lcd.setCursor(5,1);
			lcd.print("Erreur");	//erreur si condition non respectée
			erreur = true;
		}
	}
		
		
		delay(30); // pause entre 2 appuis
		
}
