#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "gp_weekend.h"

void startGrandPrix() {
    int pauseBetweenSessions = 3;      // anciennement delay_display
    int totalLapsClassic;
    int totalLapsSprint;

    // anciennement all_races
    struct races availableRaces[25] = {
        race1, race2, race3, race4, race5, race6, race7, race8, race9, race10,
        race11, race12, race13, race14, race15, race16, race17, race18, race19, race20,
        race21, race22, race23, race24, race25
    };
    int nbRaces = sizeof(availableRaces) / sizeof(availableRaces[0]);

    // anciennement all_drivers
    struct drivers contestants[20] = {
        driver1, driver2, driver3, driver4, driver5, driver6, driver7, driver8, driver9, driver10,
        driver11, driver12, driver13, driver14, driver15, driver16, driver17, driver18, driver19, driver20
    };
    int nbContestants = sizeof(contestants) / sizeof(contestants[0]);

    char userEntry[5];
    int selectedRace;
    int flushChar;

    printf("\nWelcome to the 2024 FIA Formula 1 World Championship!\n\n");

    // Tant que l'utilisateur ne rentre pas "exit"
    while (strcmp(userEntry, "exit") != 0) {
        printf("Choose one of the following Grands Prix by number:\n\n");
        for (int i = 0; i < nbRaces; i++) {
            printf("%2d | %s", i+1, availableRaces[i].name);
            if (availableRaces[i].sprint) {
                printf(" (Sprint weekend)");
            }
            printf("\n");
        }

        printf("\nGrand Prix number: ");
        scanf("%4s", userEntry);
        while ((flushChar = getchar()) != '\n' && flushChar != EOF);  // Vider le buffer

        selectedRace = atoi(userEntry) - 1;
        system("clear");

        // Vérification que la course choisie est valide
        if (selectedRace >= 0 && selectedRace < nbRaces) {
            totalLapsClassic = availableRaces[selectedRace].nb_laps;

            if (availableRaces[selectedRace].sprint) {
                totalLapsSprint = totalLapsClassic / 3;
                runWeekendSprint(contestants, pauseBetweenSessions, totalLapsClassic, totalLapsSprint);
            } else {
                runWeekendClassic(contestants, pauseBetweenSessions, totalLapsClassic);
            }

            printf("\nWould you like to run another race? (yes/no)\n");
            scanf("%4s", userEntry);
            flushChar = 0;
            while ((flushChar = getchar()) != '\n' && flushChar != EOF); // Clear input buffer

            if (strcmp(userEntry, "yes") == 0) {
                system("clear");
            } else {
                strcpy(userEntry, "exit");
            }
        }
    }
    system("clear");
    printf("\nFinal 2024 FIA Formula 1 World Championship standings:\n\n");
    showChampionshipStandings(contestants, nbContestants);
    printf("\n%s %s is your 2024 FIA Formula 1 World Champion!\n\n", contestants[0].first_name, contestants[0].last_name);
}
