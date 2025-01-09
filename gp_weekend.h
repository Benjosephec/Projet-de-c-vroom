#ifndef GP_WEEKEND_H
#define GP_WEEKEND_H

#include <stdio.h>
#include <string.h>
#include "gp_race.h"    // pour manageQualifs, manageRace
#include "gp_format.h"  // pour saveSessionResults, showStartingGrid, etc.

// --------------------
// WEEK-END CLASSIQUE
// --------------------
void runWeekendClassic(struct drivers all_drivers[20], int delay_display, int max_lap)
{
    int starting_cars[20] = {1, 11, 16, 55, 63, 44, 31, 10, 4, 81, 77, 24, 14, 18, 20, 27, 22, 3, 23, 2};
    int starting_cars_length = sizeof(starting_cars) / sizeof(starting_cars[0]);

    int cars_q2[15];
    int cars_q2_length = sizeof(cars_q2) / sizeof(cars_q2[0]);
    int cars_q3[10];
    int cars_q3_length = sizeof(cars_q3) / sizeof(cars_q3[0]);
    int starting_cars_race[20];

    int points[10] = {25, 20, 15, 10, 8, 6, 5, 3, 2, 1};

    int free_practice = 5;      // Durée simulée (ex: 5s)
    int time_q1_classic = 9;    // ex: 9s
    int time_q2_classic = 7; 
    int time_q3_classic = 5;

    char choice[4];
    int c = 0;

    // Tableaux de résultats
    struct car result[starting_cars_length];
    struct car result_free_practice[starting_cars_length];
    struct car result_race[starting_cars_length];

    // Tableaux pour récupérer les 4 meilleurs temps de la session
    struct best_time bestTimesP1[4];
    struct best_time bestTimesP2[4];
    struct best_time bestTimesP3[4];
    struct best_time bestTimesQ1[4];
    struct best_time bestTimesQ2[4];
    struct best_time bestTimesQ3[4];
    struct best_time bestTimesRace[4];

    // -------------------------------
    //        ESSAIS LIBRES (P1)
    // -------------------------------
    printf("Would you like to start the first free practice session (P1)? (yes/no)\n");
    scanf("%3s", choice);
    while ((c = getchar()) != '\n' && c != EOF);
    if (strcmp(choice, "yes") == 0) {
        manageQualifs(starting_cars, starting_cars_length, free_practice, result_free_practice, bestTimesP1);
        // Sauvegarde en fichier
        saveSessionResults("Free Practice 1 (P1)", result_free_practice, bestTimesP1, starting_cars_length, 0);
    }

    // -------------------------------
    //        ESSAIS LIBRES (P2)
    // -------------------------------
    printf("Would you like to start the second free practice session (P2)? (yes/no)\n");
    scanf("%3s", choice);
    while ((c = getchar()) != '\n' && c != EOF);
    if (strcmp(choice, "yes") == 0) {
        manageQualifs(starting_cars, starting_cars_length, free_practice, result_free_practice, bestTimesP2);
        saveSessionResults("Free Practice 2 (P2)", result_free_practice, bestTimesP2, starting_cars_length, 0);
    }

    // -------------------------------
    //        ESSAIS LIBRES (P3)
    // -------------------------------
    printf("Would you like to start the third free practice session (P3)? (yes/no)\n");
    scanf("%3s", choice);
    while ((c = getchar()) != '\n' && c != EOF);
    if (strcmp(choice, "yes") == 0) {
        manageQualifs(starting_cars, starting_cars_length, free_practice, result_free_practice, bestTimesP3);
        saveSessionResults("Free Practice 3 (P3)", result_free_practice, bestTimesP3, starting_cars_length, 0);
    }

    // -------------------------------
    //          QUALIFICATIONS
    // -------------------------------
    printf("Would you like to start the qualifications ? (yes/no)\n");
    scanf("%3s", choice);
    while ((c = getchar()) != '\n' && c != EOF);
    if (strcmp(choice, "yes") == 0) {
        // Q1
        printf("\nStart of the first qualification (Q1)\n");
        sleep(delay_display);
        manageQualifs(starting_cars, starting_cars_length, time_q1_classic, result, bestTimesQ1);
        saveSessionResults("Q1", result, bestTimesQ1, starting_cars_length, 0);

        printf("Eliminated cars : ");
        for (int i = 0; i < starting_cars_length; i++) {
            if (i >= 15 && i < 19){
                starting_cars_race[i] = result[i].number;
                printf("%d, ", result[i].number);
            }
            else if (i == 19){
                starting_cars_race[i] = result[i].number;
                printf("%d\n", result[i].number);
            } else {
                cars_q2[i] = result[i].number;
            }
        }

        // Q2
        printf("\nStart of the second qualification (Q2)\n");
        sleep(delay_display);
        manageQualifs(cars_q2, cars_q2_length, time_q2_classic, result, bestTimesQ2);
        saveSessionResults("Q2", result, bestTimesQ2, cars_q2_length, 0);

        printf("Eliminated cars : ");
        for (int i = 0; i < 15; i++) {
            if (i >= 10 && i < 14){
                starting_cars_race[i] = result[i].number;
                printf("%d, ", result[i].number);
            }
            else if (i == 14){
                starting_cars_race[i] = result[i].number;
                printf("%d\n", result[i].number);
            } else {
                cars_q3[i] = result[i].number;
            }
        }

        // Q3
        printf("\nStart of the third qualification (Q3)\n");
        sleep(delay_display);
        manageQualifs(cars_q3, cars_q3_length, time_q3_classic, result, bestTimesQ3);
        saveSessionResults("Q3", result, bestTimesQ3, cars_q3_length, 0);

        for (int i = 0; i < 10; i++) {
            starting_cars_race[i] = result[i].number;
        }

        printf("Car %d takes pole position !\n\n", starting_cars_race[0]);
        printf("---------------------------\n");
        printf("  MAIN RACE STARTING GRID\n");
        printf("---------------------------\n\n");
        showStartingGrid(starting_cars_race, starting_cars_length);
    }
    else {
        // Pas de qualifications -> on garde l'ordre initial
        for (int i = 0; i < starting_cars_length; i++) {
            starting_cars_race[i] = starting_cars[i];
        }
    }

    // -------------------------------
    //            COURSE
    // -------------------------------
    printf("Would you like to start the race ? (yes/no)\n");
    scanf("%3s", choice);
    while ((c = getchar()) != '\n' && c != EOF);
    if (strcmp(choice, "yes") == 0){
        printf("\nStart of the race\n");
        sleep(delay_display);

        manageRace(starting_cars_race, starting_cars_length, max_lap, result_race, bestTimesRace);
        // Sauvegarde de la course
        saveSessionResults("Main Race", result_race, bestTimesRace, starting_cars_length, 1);

        printf("Car %d wins the race !\n\n", result_race[0].number);

        // Attribution des points
        for(int chosen_driver = 0; chosen_driver < starting_cars_length; chosen_driver++){
            for(int current_result = 0; current_result < 10; current_result++){
                if(all_drivers[chosen_driver].number == result_race[current_result].number){
                    all_drivers[chosen_driver].points += points[current_result];
                    // Bonus point pour le meilleur tour
                    if(all_drivers[chosen_driver].number == bestTimesRace[3].car_number){
                        all_drivers[chosen_driver].points += 1;
                        printf("Car %d takes the bonus point for the fastest lap !\n\n", bestTimesRace[3].car_number);
                    }
                }
            }
        }
    }

    // Affichage final
    printf("Would you like to see the championship standings ? (yes/no)\n");
    scanf("%3s", choice);
    while ((c = getchar()) != '\n' && c != EOF);
    if (strcmp(choice, "yes") == 0) {
        system("clear");
        printf("\n2023 FIA Formula 1 World Championship standings :\n\n");
        championship_format(all_drivers, starting_cars_length);
    }
}

// --------------------
// WEEK-END SPECIAL (SPRINT)
// --------------------
void runWeekendSprint(struct drivers all_drivers[20], int delay_display, int max_lap_race, int max_lap_sprint)
{
    // Identique à 'classique', mais gère en plus la sprint race
    // ... (vous recopiez la même logique) ...
    // N’oubliez pas d’appeler saveSessionResults(...) 
    // après Q1/Q2/Q3 pour la pole, et 
    // après la sprint race, etc.
    // 
    // Par souci de concision, on ne recopie pas tout ici,
    // mais la structure est la même que runWeekendClassic,
    // avec 2 qualifications : 
    //  1) Qualifs pour la sprint
    //  2) Qualifs pour la course principale
    // puis manageRace(...) pour la sprint, manageRace(...) pour la course.
    //
    // Vous y insérez les bestTimesOut[4], etc., 
    // et vous appelez saveSessionResults("Sprint Race", ...), etc.
}

#endif // GP_WEEKEND_H
