//
// Created by Mads Mikkelsen on 11/11/2021.
//

#include <SenseHat.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <utility> // std::pair
#include <signal.h>
#include <cstdlib>
#include <stdio.h>
#include <unistd.h>

void my_handler(int s){
    printf("Caught signal %d\n",s);
    exit(1);

}

int main() {
    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = my_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);

    SenseHat carte;

    float pression;
    float temperature;
    float humidite;
    float xa,ya,za,xm,ym,zm;
    float pitch,roll,yaw;

    carte.Effacer();
    COULEUR vert  = carte.ConvertirRGB565("#009900");
    COULEUR rouge = carte.ConvertirRGB565(255,0,0);

    for (int i=0; i<8; i++){
        carte.AllumerPixel(1, i, BLEU);
        carte.AllumerPixel(0, i, rouge);
        carte.AllumerPixel(2, i, vert);
        sleep(1);
    }

    std::string filename = "test.csv";
    std::ofstream myFile(filename);
    myFile << "Temperature,Pressure,Humidity";
    myFile << "\n";

    for (int i = 0; i < 10; ++i) {
        pression    = carte.ObtenirPression();
        temperature = carte.ObtenirTemperature();
        humidite    = carte.ObtenirHumidite();

        usleep(20*1000);
        carte.ObtenirAcceleration(xa,ya,za);

        usleep(20*1000);
        carte.ObtenirOrientation(pitch,roll,yaw);

        usleep(20*1000);
        carte.ObtenirMagnetisme(xm,ym,zm);

        system("clear");
        std::cout << "pression : " << pression << " hPa"<< std::endl;
        std::cout << "Température : " << temperature << " °C" << std::endl;
        std::cout << "Humidité : " << humidite << " %" << std::endl;
        std::cout << "accélération x : " << xa << "(g) y : " << ya << "(g) z : " << za << "(g)" << std::endl;
        std::cout << "orientation pitch : " << pitch << " roll : " << roll << " yaw : " << yaw << std::endl;
        std::cout << "magnétisme x : " << xm << "(µT) y : " << ym << "(µT) z : " << zm << "(µT)" << std::endl;

        myFile << temperature << "," << pression << "," << humidite;

        myFile << "\n";

        usleep(500*1000);
    }
    // Close the file
    myFile.close();
};
