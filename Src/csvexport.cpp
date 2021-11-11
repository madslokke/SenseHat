//
// Created by Mads Mikkelsen on 11/11/2021.
//

#include <SenseHat.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <utility> // std::pair

#include <windows.h>
#include <stdio.h>

BOOL WINAPI CtrlHandler(DWORD fdwCtrlType)
{
switch (fdwCtrlType)
{
// Handle the CTRL-C signal.
case CTRL_C_EVENT:
printf("Ctrl-C event\n\n");
Beep(750, 300);
return TRUE;

// CTRL-CLOSE: confirm that the user wants to exit.
case CTRL_CLOSE_EVENT:
Beep(600, 200);
printf("Ctrl-Close event\n\n");
return TRUE;

// Pass other signals to the next handler.
case CTRL_BREAK_EVENT:
Beep(900, 200);
printf("Ctrl-Break event\n\n");
return FALSE;

case CTRL_LOGOFF_EVENT:
Beep(1000, 200);
printf("Ctrl-Logoff event\n\n");
return FALSE;

case CTRL_SHUTDOWN_EVENT:
Beep(750, 500);
printf("Ctrl-Shutdown event\n\n");
return FALSE;

default:
return FALSE;
}
}

int main() {

    if (SetConsoleCtrlHandler(CtrlHandler, TRUE))
    {
        printf("\nThe Control Handler is installed.\n");
        printf("\n -- Now try pressing Ctrl+C or Ctrl+Break, or");
        printf("\n    try logging off or closing the console...\n");
        printf("\n(...waiting in a loop for events...)\n\n");
    }
    else
    {
        printf("\nERROR: Could not set control handler");
        return 1;
    }


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
