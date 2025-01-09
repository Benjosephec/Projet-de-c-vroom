#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "weekend.h"

void championship(){
    // L'ordre de certaines déclarations a été légèrement modifié,
    // et les noms de variables ont changé.
    struct races list_races[22] = {
        race1, race2, race3, race4, race5,
        race6, race7, race8, race9, race10,
        race11, race12, race13, race14, race15,
        race16, race17, race18, race19, race20,
        race21, race22
    };
    int nb_races = sizeof(list_races) / sizeof(list_races[0]);

    struct drivers pilot_list[20] = {
        driver1, driver2, driver3, driver4, driver5,
        driver6, driver7, driver8, driver9, driver10,
        driver11, driver12, driver13, driver14, driver15,
        driver16, driver17, driver18, driver19, driver20
    };
    int nb_drivers = sizeof(pilot_list) / sizeof(pilot_list[0]);

    int show_delay = 3;
    int laps_classic;
    int laps_special;

    char user_choice[5];
    int user_choice_int;
    int flush_char;

    printf("\nWelcome to the 2024 FIA Formula 1 World Championship!\n\n");

    // On initialise user_choice pour entrer dans la boucle
    strcpy(user_choice, "");

    while (strcmp(user_choice, "exit") != 0){
        printf("Choose one of the following races by race number:\n\n");
        for (int idx_r=0; idx_r<nb_races; idx_r++){
            printf("%2d | %s", idx_r+1, list_races[idx_r].name);
            if(list_races[idx_r].sprint){
                printf(" (Sprint weekend)");
            }
            printf("\n");
        }

        printf("\nRace number: ");
        scanf("%4s", user_choice);
        while ((flush_char = getchar()) != '\n' && flush_char != EOF); // Clear input buffer

        user_choice_int = atoi(user_choice) - 1;
        system("clear");

        if(user_choice_int >= 0 && user_choice_int < nb_races){
            laps_classic = list_races[user_choice_int].nb_laps;

            if(list_races[user_choice_int].sprint){
                laps_special = laps_classic / 3;
                weekend_special(pilot_list, show_delay, laps_classic, laps_special);
            } else {
                weekend_classic(pilot_list, show_delay, laps_classic);
            }

            printf("\nWould you like to run another race? (yes/no)\n");
            scanf("%4s", user_choice);
            flush_char = 0;
            while ((flush_char = getchar()) != '\n' && flush_char != EOF); // Clear input buffer

            if(strcmp(user_choice, "yes") == 0){
                system("clear");
            }
            else{
                strcpy(user_choice, "exit");
            }
        }
    }

    system("clear");
    printf("\nFinal 2024 FIA Formula 1 World Championship standings:\n\n");
    championship_format(pilot_list, nb_drivers);
    printf("\n%s %s is your 2024 FIA Formula 1 World Champion!\n\n", pilot_list[0].first_name, pilot_list[0].last_name);
}
