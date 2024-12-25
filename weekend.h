#include <stdio.h>
#include "race.h"

void weekend_classic(struct drivers all_drivers[20], int delay_display, int max_lap){
    int starting_cars[20] = {1, 11, 16, 55, 63, 44, 31, 10, 4, 81, 77, 24, 14, 18, 20, 27, 22, 3, 23, 2};
    int starting_cars_length = sizeof(starting_cars) / sizeof(starting_cars[0]);

    int cars_q2[15];
    int cars_q2_length = sizeof(cars_q2) / sizeof(cars_q2[0]);
    int cars_q3[10];
    int cars_q3_length = sizeof(cars_q3) / sizeof(cars_q3[0]);

    int starting_cars_race[20];

    int points[10] = {25, 20, 15, 10, 8, 6, 5, 3, 2, 1};

    int free_practice = 5;
    int time_q1_classic = 9;
    int time_q2_classic = 7;
    int time_q3_classic = 5;

    char choice[4];
    int c;

    struct car result[starting_cars_length];
    struct car result_free_practice[starting_cars_length];
    struct car result_race[starting_cars_length];
    struct best_time fastest_lap[1];
    
    printf("Would you like to start the first free practice session (P1) ? (yes/no) \n");
    scanf("%3s", choice);
    while ((c = getchar()) != '\n' && c != EOF); // Clear input buffer

    if (strcmp(choice, "yes") == 0){
        run_a_qualif(starting_cars, starting_cars_length, free_practice, result_free_practice);
    }

    printf("Would you like to start the second free practice session (P2) ? (yes/no) \n");
    scanf("%3s", choice);
    c = 0;
    while ((c = getchar()) != '\n' && c != EOF); // Clear input buffer

    if (strcmp(choice, "yes") == 0){
        run_a_qualif(starting_cars, starting_cars_length, free_practice, result_free_practice);
    }

    printf("Would you like to start the third free practice session (P3) ? (yes/no) \n");
    scanf("%3s", choice);
    c = 0;
    while ((c = getchar()) != '\n' && c != EOF); // Clear input buffer

    if (strcmp(choice, "yes") == 0){
        run_a_qualif(starting_cars, starting_cars_length, free_practice, result_free_practice);
    }

    printf("Would you like to start the qualifications ? (yes/no) \n");
    scanf("%3s", choice);
    c = 0;
    while ((c = getchar()) != '\n' && c != EOF); // Clear input buffer

    if (strcmp(choice, "yes") == 0){
        printf("\nStart of the first qualification (Q1) \n");
        sleep(delay_display);

        run_a_qualif(starting_cars, starting_cars_length, time_q1_classic, result);
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

        printf("\nStart of the second qualification (Q2) \n");
        sleep(delay_display);
        
        run_a_qualif(cars_q2, cars_q2_length, time_q2_classic, result);
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

        printf("\nStart of the third qualification (Q3) \n");
        sleep(delay_display);
    
        run_a_qualif(cars_q3, cars_q3_length, time_q3_classic, result);
        for (int i = 0; i < 10; i++) {
            starting_cars_race[i] = result[i].number;
        }

        printf("Car %d takes pole position ! \n\n", starting_cars_race[0]);
        printf("---------------------------\n");
        printf("  MAIN RACE STARTING GRID\n");
        printf("---------------------------\n\n");
        starting_grid_format(starting_cars_race, starting_cars_length);
    } else {
        for (int i = 0; i < starting_cars_length; i++) {
            starting_cars_race[i] = starting_cars[i];
        }
    }

    printf("Would you like to start the race ? (yes/no) \n");
    scanf("%3s", choice);
    c = 0;
    while ((c = getchar()) != '\n' && c != EOF); // Clear input buffer

    if (strcmp(choice, "yes") == 0){
        printf("\nStart of the race \n");
        sleep(delay_display);

        run_a_race(starting_cars_race, starting_cars_length, max_lap, result_race, fastest_lap);

        printf("Car %d wins the race ! \n\n", result_race[0].number);

        for(int chosen_driver=0; chosen_driver<starting_cars_length; chosen_driver++){
            for(int current_result=0; current_result<10; current_result++){
                if(all_drivers[chosen_driver].number == result_race[current_result].number){
                    all_drivers[chosen_driver].points += points[current_result]; // Add points to the drivers depending on their position

                    if(all_drivers[chosen_driver].number == fastest_lap[0].car_number){
                        all_drivers[chosen_driver].points += 1; // Add 1 point for the fastest lap if the driver is in the top 10
                        printf("Car %d takes the bonus point for the fastest lap ! \n\n", fastest_lap[0].car_number);
                    }
                }
            }
        }
    }

    printf("Would you like to see the championship standings ? (yes/no) \n");
    scanf("%3s", choice);
    c = 0;
    while ((c = getchar()) != '\n' && c != EOF); // Clear input buffer

    if (strcmp(choice, "yes") == 0){
        system("clear");
        printf("\n2023 FIA Formula 1 World Championship standings : \n\n");
        championship_format(all_drivers, starting_cars_length);
    }
}

void weekend_special(struct drivers all_drivers[20], int delay_display, int max_lap_race, int max_lap_sprint){
    int starting_cars[20] = {1, 11, 16, 55, 63, 44, 31, 10, 4, 81, 77, 24, 14, 18, 20, 27, 22, 3, 23, 2};
    int starting_cars_length = sizeof(starting_cars) / sizeof(starting_cars[0]);

    int cars_q2[15];
    int cars_q2_length = sizeof(cars_q2) / sizeof(cars_q2[0]);
    int cars_q3[10];
    int cars_q3_length = sizeof(cars_q3) / sizeof(cars_q3[0]);

    int starting_cars_race[20];
    int starting_cars_sprint[20];

    int points_classic[10] = {25, 20, 15, 10, 8, 6, 5, 3, 2, 1};
    int points_sprint[8] = {8, 7, 6, 5, 4, 3, 2, 1};
    
    int free_practice = 5;

    int TIME_Q1_CLASSIC = 9;
    int TIME_Q2_CLASSIC = 7;
    int TIME_Q3_CLASSIC = 5;

    int TIME_Q1_SPECIAL = 6;
    int TIME_Q2_SPECIAL = 5;
    int TIME_Q3_SPECIAL = 4;

    char choice[4];
    int c;

    struct car result[starting_cars_length];
    struct car result_free_practice[starting_cars_length];
    struct car result_race[starting_cars_length];
    struct car result_sprint[starting_cars_length];
    struct best_time fastest_lap_classic[1];
    struct best_time fastest_lap_sprint[1];
    
    printf("Would you like to start the free practice session (P1) ? (yes/no) \n");
    scanf("%3s", choice);
    while ((c = getchar()) != '\n' && c != EOF); // Clear input buffer

    if (strcmp(choice, "yes") == 0){
        run_a_qualif(starting_cars, starting_cars_length, free_practice, result_free_practice);
    }

    /******************************************************/
    /******************      CLASSIC      *****************/
    /******************************************************/
    printf("Would you like to start the qualifications ? (yes/no) \n");
    scanf("%3s", choice);
    c = 0;
    while ((c = getchar()) != '\n' && c != EOF); // Clear input buffer

    if (strcmp(choice, "yes") == 0){
        printf("\nStart of the first qualification (Q1) \n");
        sleep(delay_display);

        run_a_qualif(starting_cars, starting_cars_length, TIME_Q1_CLASSIC, result);
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

        printf("\nStart of the second qualification (Q2) \n");
        sleep(delay_display);
        
        run_a_qualif(cars_q2, cars_q2_length, TIME_Q2_CLASSIC, result);
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

        printf("\nStart of the third qualification (Q3) \n");
        sleep(delay_display);
    
        run_a_qualif(cars_q3, cars_q3_length, TIME_Q3_CLASSIC, result);
        for (int i = 0; i < 10; i++) {
            starting_cars_race[i] = result[i].number;
        }

        printf("Car %d takes pole position ! \n\n", starting_cars_race[0]);
        printf("---------------------------\n");
        printf("  MAIN RACE STARTING GRID\n");
        printf("---------------------------\n\n");
        starting_grid_format(starting_cars_race, starting_cars_length);
    } else {
        for (int i = 0; i < starting_cars_length; i++) {
            starting_cars_race[i] = starting_cars[i];
        }
    }

    /******************************************************/
    /******************      SPRINT       *****************/
    /******************************************************/
    printf("Would you like to start the sprint qualifications ? (yes/no) \n");
    scanf("%3s", choice);
    c = 0;
    while ((c = getchar()) != '\n' && c != EOF); // Clear input buffer

    if (strcmp(choice, "yes") == 0){
        printf("\nStart of the first qualification (Q1) \n");
        sleep(delay_display);

        run_a_qualif(starting_cars, starting_cars_length, TIME_Q1_SPECIAL, result);
        printf("Eliminated cars : ");
        for (int i = 0; i < starting_cars_length; i++) {
            if (i >= 15 && i < 19){
                starting_cars_sprint[i] = result[i].number;
                printf("%d, ", result[i].number);
            }
            else if (i == 19){
                starting_cars_sprint[i] = result[i].number;
                printf("%d\n", result[i].number);
            } else {
                cars_q2[i] = result[i].number;
            }
        }

        printf("\nStart of the second qualification (Q2) \n");
        sleep(delay_display);
        
        run_a_qualif(cars_q2, cars_q2_length, TIME_Q2_SPECIAL, result);
        printf("Eliminated cars : ");
        for (int i = 0; i < 15; i++) {
            if (i >= 10 && i < 14){
                starting_cars_sprint[i] = result[i].number;
                printf("%d, ", result[i].number);
            }
            else if (i == 14){
                starting_cars_sprint[i] = result[i].number;
                printf("%d\n", result[i].number);
            } else {
                cars_q3[i] = result[i].number;
            }
        }

        printf("\nStart of the third qualification (Q3) \n");
        sleep(delay_display);
    
        run_a_qualif(cars_q3, cars_q3_length, TIME_Q3_SPECIAL, result);
        for (int i = 0; i < 10; i++) {
            starting_cars_sprint[i] = result[i].number;
        }
        printf("Car %d takes pole position ! \n\n", starting_cars_sprint[0]);
        printf("---------------------------\n");
        printf("  SPRINT RACE STARTING GRID\n");
        printf("---------------------------\n\n");
        starting_grid_format(starting_cars_sprint, starting_cars_length);
    } else {
        for (int i = 0; i < starting_cars_length; i++) {
            starting_cars_sprint[i] = starting_cars[i];
        }
    }

    printf("Would you like to start the sprint race ? (yes/no) \n");
    scanf("%3s", choice);
    c = 0;
    while ((c = getchar()) != '\n' && c != EOF); // Clear input buffer

    if (strcmp(choice, "yes") == 0){
        sleep(delay_display);
        printf("\nStart of the sprint race \n");

        run_a_race(starting_cars_sprint, starting_cars_length, max_lap_sprint, result_sprint, fastest_lap_sprint);

        printf("Car %d wins the sprint race ! \n\n", result_sprint[0].number);

        for(int chosen_driver=0; chosen_driver<starting_cars_length; chosen_driver++){
            for(int current_result=0; current_result<8; current_result++){
                if(all_drivers[chosen_driver].number == result_sprint[current_result].number){
                    all_drivers[chosen_driver].points += points_sprint[current_result]; // Add points to the drivers depending on their position
                }
            }
        }
    }

    printf("Would you like to see the championship standings ? (yes/no) \n");
    scanf("%3s", choice);
    c = 0;
    while ((c = getchar()) != '\n' && c != EOF); // Clear input buffer

    if (strcmp(choice, "yes") == 0){
        system("clear");
        printf("\n2023 FIA Formula 1 World Championship standings : \n\n");
        championship_format(all_drivers, starting_cars_length);
    }

    /******************************************************/
    /******************       RACE        *****************/
    /******************************************************/
    printf("\nWould you like to start the race ? (yes/no) \n");
    scanf("%3s", choice);
    c = 0;
    while ((c = getchar()) != '\n' && c != EOF); // Clear input buffer

    if (strcmp(choice, "yes") == 0){
        printf("\nStart of the race \n");
        sleep(delay_display);

        run_a_race(starting_cars_race, starting_cars_length, max_lap_race, result_race, fastest_lap_classic);

        printf("Car %d wins the race ! \n\n", result_race[0].number);

        for(int chosen_driver=0; chosen_driver<starting_cars_length; chosen_driver++){
            for(int current_result=0; current_result<10; current_result++){
                if(all_drivers[chosen_driver].number == result_race[current_result].number){
                    all_drivers[chosen_driver].points += points_classic[current_result]; // Add points to the drivers depending on their position

                    if(all_drivers[chosen_driver].number == fastest_lap_classic[0].car_number){
                        all_drivers[chosen_driver].points += 1; // Add 1 point for the fastest lap if the driver is in the top 10
                        printf("Car %d takes the bonus point for the fastest lap ! \n\n", fastest_lap_classic[0].car_number);
                    }
                }
            }
        }
    }

    printf("Would you like to see the championship standings ? (yes/no) \n");
    scanf("%3s", choice);
    c = 0;
    while ((c = getchar()) != '\n' && c != EOF); // Clear input buffer

    if (strcmp(choice, "yes") == 0){
        system("clear");
        printf("\n2023 FIA Formula 1 World Championship standings : \n\n");
        championship_format(all_drivers, starting_cars_length);
    }
}
