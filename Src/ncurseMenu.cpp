
#include <SenseHat.h>                           //Nødvendige libraries
#include <string>                               //
#include <vector>                               //
#include <iostream>                             //
#include <ncurses.h>                            //

#define ENTRY_Y_OFFSET 3

using namespace std;                            //Vi arbjeder i std's namespace, da ingen andre namespaces er nødvendige her.

enum { TEMPERATURE = 0, HUMIDITY = 1, PRESSURE = 2 };                                        //enum til indeksering af sensor array

enum { MAIN_MENU = 0, TEMP_MENU = 1, HUM_MENU = 2, PRES_MENU = 3 };                            //enum til indeksering af menuer

vector<vector<string>> menu_entries;                                                //vector af vectorer af strings til menupunkter

vector<string> main_menu = { "Get temperature", "Get humidity", "Get pressure" };     //main menu punkter
vector<string> temp_menu = { "The temperature is: ", "Degrees C" };                   //temperaturmenupunkter
vector<string> hum_menu = { "The humidity is: ", "%" };                              //fugtmenupunkter
vector<string> pres_menu = { "The atmospheric pressure is: ", "hPa" };                //Trykmenupunkter


void draw_menu(WINDOW* win, int menu, float* data)                                  //funktion til visning af aktuel menu
{
    wborder(win, '*', '*', '*', '*', '*', '*', '*', '*');		                            //window border

    if (menu < 1)                                                                    //hvis main-menu                                                                     
    {
        for (int i = 0; i < menu_entries[menu].size(); i++)                          //iterér gennem antal menupunkter
        {

            wmove(win, i * 2 + ENTRY_Y_OFFSET, 10);                                     // print main-menu-punter
            waddstr(win, "[");
            wmove(win, i * 2 + ENTRY_Y_OFFSET, 12);
            waddstr(win, menu_entries[menu][i].c_str());                            //.c_str() omdan std::string til char *
            wmove(win, 15, 10);
            waddstr(win, "Press q to quit");
        }
    }

    else
    {
        wmove(win, ENTRY_Y_OFFSET, 10);                                                                              //Ellers print undermenu fra offset ENTRY_Y_OFFSET
        wprintw(win, "%s %.2f %s", menu_entries[menu][0].c_str(), data[menu - 1], menu_entries[menu][1].c_str());      //Forventer data sandwich'ed ind mellem to strings
    }

}


void get_env_sensor_data(SenseHat* s, float* env)  //Funktion der sampler alle sensorer fordi, hvorfor ikke
{                                                  //Giver pointer til SenseHat objekt med da vi har initialiseret vores instans inde i main.
                                                   //Gemmer by-reference i float-array
    env[TEMPERATURE] = s->ObtenirTemperature();    //Temperatur
    env[HUMIDITY] = s->ObtenirHumidite();          //Fugtighed
    env[PRESSURE] = s->ObtenirPression();          //Tryk
}


int main()
{
    float env_sensors[3];                       //float array med tre pladser

    menu_entries.push_back(main_menu);          //Skubber alle menuer ind i den overordnede vector
    menu_entries.push_back(temp_menu);
    menu_entries.push_back(hum_menu);
    menu_entries.push_back(pres_menu);

    SenseHat carte;                             //Opretter instans af SenseHat. initialiserer gennem dens constructor

    carte.Effacer();                            //Slukker alle LED'er i LED-matrix

    initscr();                                  //Overtager terminalens screenbuffer
    cbreak();                                   //Gør knapper til "actions" ved ikke at kræve tryk på enter for at tage effekt
    noecho();                                   //Viser ikke indtastede karakterer
    refresh();                                  //Beder screenbufferen om at outputte (ingenting)


    WINDOW* win = newwin(24, 80, 0, 0);         //Objekt der håndterer ét vindue


    keypad(win, TRUE);	                        //Giver adgang til f. eks. pil op og ned på tastatur

    start_color();                              //giver adgang til at bruge farver
    init_pair(2, COLOR_BLACK, COLOR_CYAN);      //primære farve
    init_pair(3, COLOR_RED, COLOR_GREEN);       //Farve til BACK-knap
    wattron(win, COLOR_PAIR(2));                //Tænder for color-par 1 i win

    draw_menu(win, MAIN_MENU, env_sensors);     //Viser main menu (env_sensors bruges ikke i main menu, så kan være uinitialiserede)
    int cursor_position = 0;                    //Variabel der bruges til at indekserer undermenuer
    int main_menu_entries = menu_entries[MAIN_MENU].size();                  //giver antal undermenuer der kan vælges
    int whatMenuAmIIn = MAIN_MENU;                                           //Starter med at stå i main menu

    curs_set(1);                                                             //Cursoren vises
    wmove(win, 3, 11);  // window, y, x                                      //Cursoren står ud for fårste punkt på menuen


    wrefresh(win);		// Refresh window

    while (1)
    {
        int key = wgetch(win);                  //Læs keyboard input

        if (key == 'q') break;                   //Hvis der er trykket på q. afslut programmet

        switch (key)                             //Hvilken tast er der trykket på
        {
        case KEY_UP:
            if (cursor_position - 1 >= 0) --cursor_position;    //Ryk cursoren en plads op hvis, ikke den allerede står øverst
            break;

        case KEY_DOWN:
            if (cursor_position + 1 < main_menu_entries) ++cursor_position;      //Ryk cursoren en plads ned hvis, ikke den allerede står nederst
            break;

        case '\n':                                                             //Hvis der er trykket på Return-tasten
            get_env_sensor_data(&carte, env_sensors);                           //Læs sensordata
            whatMenuAmIIn = whatMenuAmIIn == MAIN_MENU ? cursor_position + 1 : MAIN_MENU;  //Hvis du har trykket på Return fra main menu, sæt variabel. hvilken menu skal vi ind i.
            cursor_position = 0;                                                //Sæt cursor variablen til øverste menupunkt
            wclear(win);                                                        //Slet main menu
            wattron(win, COLOR_PAIR(2));                                        //Aktiver farver

            if (whatMenuAmIIn != MAIN_MENU)                                      //Hvis ikke der er valgt main menu.
            {
                draw_menu(win, whatMenuAmIIn, env_sensors);                     //Print indholdet af vores menu vector i index whatMenuAmIIn (med sensor data)
                curs_set(0);                                                    //Sluk cursor
                wmove(win, 15, 12);                                             //Skriv < BACK > med color-par 3 i bunden af skærmen
                wattron(win, COLOR_PAIR(3));
                wprintw(win, "< BACK >");
                wattroff(win, COLOR_PAIR(3));
            }

            else
            {
                whatMenuAmIIn = MAIN_MENU;
                draw_menu(win, MAIN_MENU, env_sensors);                          //Print main menu igen 
                curs_set(1);                                                     //Tæn cursor
            }

        }

        if (whatMenuAmIIn < 1) wmove(win, 3 + cursor_position * 2, 11);              //Hvis i main menu hvis cursoren som indikator for hvilket menupunkt der ståes ud for.
    }

    delwin(win);                                                                    //Slet WINDOW objektet
    endwin();                                                                       //Frigiv terminalens screen buffer

    return 0;                                                                       //Afslut programmet med exit code success
}