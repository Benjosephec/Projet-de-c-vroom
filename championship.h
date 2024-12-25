#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "weekend.h"

void championship(){
    int delay_display = 3;
    int max_lap_classic;
    int max_lap_special;

    struct races all_races[25] = {race1, race2, race3, race4, race5, race6, race7, race8, race9, race10, race11, race12, race13, race14, race15, race16, race17, race18, race19, race20, race21, race22, race23, race24, race25};
    int races_length = sizeof(all_races) / sizeof(all_races[0]);

    struct drivers all_drivers[20] = {driver1, driver2, driver3, driver4, driver5, driver6, driver7, driver8, driver9, driver10, driver11, driver12, driver13, driver14, driver15, driver16, driver17, driver18, driver19, driver20};
    int drivers_length = sizeof(all_drivers) / sizeof(all_drivers[0]);

    char choice[5];
    int choice_int;
    int c;

    printf("\nWelcome to the 2024 FIA Formula 1 World Championship !\n\n");

    while (strcmp(choice, "exit") != 0){
        printf("Choose one of the following races by race number : \n\n");
        for (int current_race=0; current_race<races_length; current_race++){
            printf("%2d | %s", current_race+1, all_races[current_race].name);
            if(all_races[current_race].sprint){
                printf(" (Sprint weekend)");
            }
            printf("\n");
        }

        printf("\nRace number : ");
        scanf("%4s", choice);
        while ((c = getchar()) != '\n' && c != EOF); // Clear input buffer
        
        choice_int = atoi(choice)-1;
        system("clear");

        if(choice_int >= 0 && choice_int < races_length){
            max_lap_classic = all_races[choice_int].nb_laps;

            if(all_races[choice_int].sprint){
                max_lap_special = max_lap_classic/3;
                weekend_special(all_drivers, delay_display, max_lap_classic, max_lap_special);
            }else{
                weekend_classic(all_drivers, delay_display, max_lap_classic);
            }

            printf("\nWould you like to run another race ? (yes/no)\n");
            scanf("%4s", choice);
            c = 0;
            while ((c = getchar()) != '\n' && c != EOF); // Clear input buffer

            if(strcmp(choice, "yes") == 0){
                system("clear");
            }
            else{
                strcpy(choice, "exit");
            }
        }
    }
    system("clear");
    printf("\nFinal 2024 FIA Formula 1 World Championship standings : \n\n");
    championship_format(all_drivers, drivers_length);
    printf("\n%s %s is your 2024 FIA Formula 1 World Champion !\n\n", all_drivers[0].first_name, all_drivers[0].last_name);
}
