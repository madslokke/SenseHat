 /***********************************************************************************
/*!
    \file         text.cpp
    \author    	  Philippe SIMIER (Touchard Wahington le Mans)
    \license      GPL2 (see license.txt)
    \date         25 février 2018
    \brief        Programme exemple pour l'affichage d'un message
    \details      Mise en oeuvre de l'opérateur de flux <<
		  pour afficher des messages sur une carte SenseHat

    \compilation: g++ text.cpp -l SenseHat -l RTIMULib -o text
    \version      v1.0
/***********************************************************************************/

#include <SenseHat.h>
#include <iostream>
#include <string>


using namespace std;

int main() {

    SenseHat carte;
    string message;

    carte.Effacer();
    cout << "Entrer votre message : ";
    getline(cin, message);

    int i = 0;

    while(1){

        i += 10;
        if (i > 255) {
            i = 0;
        }
        carte << setcouleur(carte.ConvertirRGB565(255, 105, i)) << setrotation(270);
        carte << message << flush;
        sleep(1);
    }
    return 0;
}

