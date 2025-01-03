#include <stdio.h>
#include "gp_race.h"

// Week-end classique
void runWeekendClassic(struct drivers participants[20], int waitTime, int maxLap) {
    int initialCars[20] = {1, 11, 16, 55, 63, 44, 31, 10, 4, 81, 77, 24, 14, 18, 20, 27, 22, 3, 23, 2};
    int nbCars = sizeof(initialCars) / sizeof(initialCars[0]);

    int carsQ2[15];
    int carsQ2_len = sizeof(carsQ2) / sizeof(carsQ2[0]);
    int carsQ3[10];
    int carsQ3_len = sizeof(carsQ3) / sizeof(carsQ3[0]);
    int gridForRace[20];

    int scoring[10] = {25, 20, 15, 10, 8, 6, 5, 3, 2, 1};

    int freePracticeSec = 5;
    int q1TimeClassic   = 9;
    int q2TimeClassic   = 7;
    int q3TimeClassic   = 5;

    char choice[4];
    int flush;

    struct car tmpResult[nbCars];
    struct car freePracticeRes[nbCars];
    struct car raceResult[nbCars];
    struct best_time bestLap[1];

    // Séances d'essais libres
    printf("Would you like to start the first free practice session (P1)? (yes/no)\n");
    scanf("%3s", choice);
    while ((flush = getchar()) != '\n' && flush != EOF);
    if (strcmp(choice, "yes") == 0) {
        manageQualifs(initialCars, nbCars, freePracticeSec, freePracticeRes);
    }

    printf("Would you like to start the second free practice session (P2)? (yes/no)\n");
    scanf("%3s", choice);
    flush = 0;
    while ((flush = getchar()) != '\n' && flush != EOF);
    if (strcmp(choice, "yes") == 0) {
        manageQualifs(initialCars, nbCars, freePracticeSec, freePracticeRes);
    }

    printf("Would you like to start the third free practice session (P3)? (yes/no)\n");
    scanf("%3s", choice);
    flush = 0;
    while ((flush = getchar()) != '\n' && flush != EOF);
    if (strcmp(choice, "yes") == 0) {
        manageQualifs(initialCars, nbCars, freePracticeSec, freePracticeRes);
    }

    // Qualifications
    printf("Would you like to start the qualifications? (yes/no)\n");
    scanf("%3s", choice);
    flush = 0;
    while ((flush = getchar()) != '\n' && flush != EOF);

    if (strcmp(choice, "yes") == 0) {
        printf("\nStart of the first qualification (Q1)\n");
        sleep(waitTime);

        manageQualifs(initialCars, nbCars, q1TimeClassic, tmpResult);
        printf("Eliminated cars: ");
        for (int i = 0; i < nbCars; i++) {
            if (i >= 15 && i < 19) {
                gridForRace[i] = tmpResult[i].number;
                printf("%d, ", tmpResult[i].number);
            } else if (i == 19) {
                gridForRace[i] = tmpResult[i].number;
                printf("%d\n", tmpResult[i].number);
            } else {
                carsQ2[i] = tmpResult[i].number;
            }
        }

        printf("\nStart of the second qualification (Q2)\n");
        sleep(waitTime);

        manageQualifs(carsQ2, carsQ2_len, q2TimeClassic, tmpResult);
        printf("Eliminated cars: ");
        for (int i = 0; i < 15; i++) {
            if (i >= 10 && i < 14) {
                gridForRace[i] = tmpResult[i].number;
                printf("%d, ", tmpResult[i].number);
            } else if (i == 14) {
                gridForRace[i] = tmpResult[i].number;
                printf("%d\n", tmpResult[i].number);
            } else {
                carsQ3[i] = tmpResult[i].number;
            }
        }

        printf("\nStart of the third qualification (Q3)\n");
        sleep(waitTime);

        manageQualifs(carsQ3, carsQ3_len, q3TimeClassic, tmpResult);
        for (int i = 0; i < 10; i++) {
            gridForRace[i] = tmpResult[i].number;
        }

        printf("Car %d takes pole position!\n\n", gridForRace[0]);
        printf("---------------------------\n");
        printf("  MAIN RACE STARTING GRID\n");
        printf("---------------------------\n\n");
        showStartingGrid(gridForRace, nbCars);
    } else {
        for (int i = 0; i < nbCars; i++) {
            gridForRace[i] = initialCars[i];
        }
    }

    // Course principale
    printf("Would you like to start the race? (yes/no)\n");
    scanf("%3s", choice);
    flush = 0;
    while ((flush = getchar()) != '\n' && flush != EOF);

    if (strcmp(choice, "yes") == 0) {
        printf("\nStart of the race\n");
        sleep(waitTime);

        manageRace(gridForRace, nbCars, maxLap, raceResult, bestLap);
        printf("Car %d wins the race!\n\n", raceResult[0].number);

        // Attribution des points
        for (int d = 0; d < nbCars; d++) {
            for (int pos = 0; pos < 10; pos++) {
                if (participants[d].number == raceResult[pos].number) {
                    participants[d].points += scoring[pos];
                    // Bonus point
                    if (participants[d].number == bestLap[0].car_number) {
                        participants[d].points += 1;
                        printf("Car %d takes the bonus point for the fastest lap!\n\n", bestLap[0].car_number);
                    }
                }
            }
        }
    }

    // Affichage du championnat
    printf("Would you like to see the championship standings? (yes/no)\n");
    scanf("%3s", choice);
    flush = 0;
    while ((flush = getchar()) != '\n' && flush != EOF);
    if (strcmp(choice, "yes") == 0) {
        system("clear");
        printf("\n2023 FIA Formula 1 World Championship standings:\n\n");
        showChampionshipStandings(participants, nbCars);
    }
}

// Week-end spécial (sprint)
void runWeekendSprint(struct drivers participants[20], int waitTime, int lapsRace, int lapsSprint) {
    int initialCars[20] = {1, 11, 16, 55, 63, 44, 31, 10, 4, 81, 77, 24, 14, 18, 20, 27, 22, 3, 23, 2};
    int nbCars = sizeof(initialCars) / sizeof(initialCars[0]);

    int carsQ2[15];
    int carsQ2_len = sizeof(carsQ2) / sizeof(carsQ2[0]);
    int carsQ3[10];
    int carsQ3_len = sizeof(carsQ3) / sizeof(carsQ3[0]);

    int gridMainRace[20];
    int gridSprint[20];

    int pointsMain[10]   = {25, 20, 15, 10, 8, 6, 5, 3, 2, 1};
    int pointsSprint[8]  = {8,  7,  6,  5, 4, 3, 2, 1};

    int freePracticeSec  = 5;
    int q1TimeClassic    = 9;
    int q2TimeClassic    = 7;
    int q3TimeClassic    = 5;

    int q1TimeSprint     = 6;
    int q2TimeSprint     = 5;
    int q3TimeSprint     = 4;

    char choice[4];
    int flush;

    struct car tmpResult[nbCars];
    struct car freePracticeRes[nbCars];
    struct car mainRaceResult[nbCars];
    struct car sprintResult[nbCars];
    struct best_time bestLapMain[1];
    struct best_time bestLapSprint[1];

    // Séance d'essais libres
    printf("Would you like to start the free practice session (P1)? (yes/no)\n");
    scanf("%3s", choice);
    while ((flush = getchar()) != '\n' && flush != EOF);
    if (strcmp(choice, "yes") == 0) {
        manageQualifs(initialCars, nbCars, freePracticeSec, freePracticeRes);
    }

    /******************************************************/
    /******************      CLASSIC      *****************/
    /******************************************************/
    printf("Would you like to start the qualifications? (yes/no)\n");
    scanf("%3s", choice);
    flush = 0;
    while ((flush = getchar()) != '\n' && flush != EOF);

    if (strcmp(choice, "yes") == 0) {
        printf("\nStart of the first qualification (Q1)\n");
        sleep(waitTime);

        manageQualifs(initialCars, nbCars, q1TimeClassic, tmpResult);
        printf("Eliminated cars: ");
        for (int i = 0; i < nbCars; i++) {
            if (i >= 15 && i < 19) {
                gridMainRace[i] = tmpResult[i].number;
                printf("%d, ", tmpResult[i].number);
            } else if (i == 19) {
                gridMainRace[i] = tmpResult[i].number;
                printf("%d\n", tmpResult[i].number);
            } else {
                carsQ2[i] = tmpResult[i].number;
            }
        }

        printf("\nStart of the second qualification (Q2)\n");
        sleep(waitTime);

        manageQualifs(carsQ2, carsQ2_len, q2TimeClassic, tmpResult);
        printf("Eliminated cars: ");
        for (int i = 0; i < 15; i++) {
            if (i >= 10 && i < 14) {
                gridMainRace[i] = tmpResult[i].number;
                printf("%d, ", tmpResult[i].number);
            } else if (i == 14) {
                gridMainRace[i] = tmpResult[i].number;
                printf("%d\n", tmpResult[i].number);
            } else {
                carsQ3[i] = tmpResult[i].number;
            }
        }

        printf("\nStart of the third qualification (Q3)\n");
        sleep(waitTime);

        manageQualifs(carsQ3, carsQ3_len, q3TimeClassic, tmpResult);
        for (int i = 0; i < 10; i++) {
            gridMainRace[i] = tmpResult[i].number;
        }

        printf("Car %d takes pole position!\n\n", gridMainRace[0]);
        printf("---------------------------\n");
        printf("  MAIN RACE STARTING GRID\n");
        printf("---------------------------\n\n");
        showStartingGrid(gridMainRace, nbCars);
    } else {
        for (int i = 0; i < nbCars; i++) {
            gridMainRace[i] = initialCars[i];
        }
    }

    /******************************************************/
    /******************      SPRINT       *****************/
    /******************************************************/
    printf("Would you like to start the sprint qualifications? (yes/no)\n");
    scanf("%3s", choice);
    flush = 0;
    while ((flush = getchar()) != '\n' && flush != EOF);

    if (strcmp(choice, "yes") == 0) {
        printf("\nStart of the first qualification (Q1)\n");
        sleep(waitTime);

        manageQualifs(initialCars, nbCars, q1TimeSprint, tmpResult);
        printf("Eliminated cars: ");
        for (int i = 0; i < nbCars; i++) {
            if (i >= 15 && i < 19) {
                gridSprint[i] = tmpResult[i].number;
                printf("%d, ", tmpResult[i].number);
            } else if (i == 19) {
                gridSprint[i] = tmpResult[i].number;
                printf("%d\n", tmpResult[i].number);
            } else {
                carsQ2[i] = tmpResult[i].number;
            }
        }

        printf("\nStart of the second qualification (Q2)\n");
        sleep(waitTime);

        manageQualifs(carsQ2, carsQ2_len, q2TimeSprint, tmpResult);
        printf("Eliminated cars: ");
        for (int i = 0; i < 15; i++) {
            if (i >= 10 && i < 14) {
                gridSprint[i] = tmpResult[i].number;
                printf("%d, ", tmpResult[i].number);
            } else if (i == 14) {
                gridSprint[i] = tmpResult[i].number;
                printf("%d\n", tmpResult[i].number);
            } else {
                carsQ3[i] = tmpResult[i].number;
            }
        }

        printf("\nStart of the third qualification (Q3)\n");
        sleep(waitTime);

        manageQualifs(carsQ3, carsQ3_len, q3TimeSprint, tmpResult);
        for (int i = 0; i < 10; i++) {
            gridSprint[i] = tmpResult[i].number;
        }
        printf("Car %d takes pole position!\n\n", gridSprint[0]);
        printf("---------------------------\n");
        printf("  SPRINT RACE STARTING GRID\n");
        printf("---------------------------\n\n");
        showStartingGrid(gridSprint, nbCars);
    } else {
        for (int i = 0; i < nbCars; i++) {
            gridSprint[i] = initialCars[i];
        }
    }

    // Sprint
    printf("Would you like to start the sprint race? (yes/no)\n");
    scanf("%3s", choice);
    flush = 0;
    while ((flush = getchar()) != '\n' && flush != EOF);

    if (strcmp(choice, "yes") == 0) {
        sleep(waitTime);
        printf("\nStart of the sprint race\n");

        manageRace(gridSprint, nbCars, lapsSprint, sprintResult, bestLapSprint);
        printf("Car %d wins the sprint race!\n\n", sprintResult[0].number);

        // Attribution de points sprint
        for (int d = 0; d < nbCars; d++) {
            for (int pos = 0; pos < 8; pos++) {
                if (participants[d].number == sprintResult[pos].number) {
                    participants[d].points += pointsSprint[pos];
                }
            }
        }
    }

    printf("Would you like to see the championship standings? (yes/no)\n");
    scanf("%3s", choice);
    flush = 0;
    while ((flush = getchar()) != '\n' && flush != EOF);
    if (strcmp(choice, "yes") == 0) {
        system("clear");
        printf("\n2023 FIA Formula 1 World Championship standings:\n\n");
        showChampionshipStandings(participants, nbCars);
    }

    /******************************************************/
    /******************       RACE        *****************/
    /******************************************************/
    printf("\nWould you like to start the race? (yes/no)\n");
    scanf("%3s", choice);
    flush = 0;
    while ((flush = getchar()) != '\n' && flush != EOF);

    if (strcmp(choice, "yes") == 0) {
        printf("\nStart of the race\n");
        sleep(waitTime);

        manageRace(gridMainRace, nbCars, lapsRace, mainRaceResult, bestLapMain);
        printf("Car %d wins the race!\n\n", mainRaceResult[0].number);

        // Attribution de points main
        for (int d = 0; d < nbCars; d++) {
            for (int pos = 0; pos < 10; pos++) {
                if (participants[d].number == mainRaceResult[pos].number) {
                    participants[d].points += pointsMain[pos];
                    if (participants[d].number == bestLapMain[0].car_number) {
                        participants[d].points += 1;
                        printf("Car %d takes the bonus point for the fastest lap!\n\n", bestLapMain[0].car_number);
                    }
                }
            }
        }
    }

    // Classement
    printf("Would you like to see the championship standings? (yes/no)\n");
    scanf("%3s", choice);
    flush = 0;
    while ((flush = getchar()) != '\n' && flush != EOF);
    if (strcmp(choice, "yes") == 0) {
        system("clear");
        printf("\n2023 FIA Formula 1 World Championship standings:\n\n");
        showChampionshipStandings(participants, nbCars);
    }
}
