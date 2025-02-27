#include <stdio.h>
#include "race.h"

void weekend_classic(struct drivers all_drivers[20], int show_delay, int max_lap){
    int initial_cars[20] = {1, 11, 16, 55, 63, 44, 31, 10, 4, 81,
                            77, 24, 14, 18, 20, 27, 22, 3, 23, 2};
    int len_initial_cars = sizeof(initial_cars) / sizeof(initial_cars[0]);

    int cars_q2[15];
    int len_cars_q2 = sizeof(cars_q2) / sizeof(cars_q2[0]);
    int cars_q3[10];
    int len_cars_q3 = sizeof(cars_q3) / sizeof(cars_q3[0]);

    int grid_race[20];

    int points_awarded[10] = {25, 20, 15, 10, 8, 6, 5, 3, 2, 1};

    int time_free_practice = 5;
    int q1_classic = 9;
    int q2_classic = 7;
    int q3_classic = 5;

    char user_decision[4];
    int flush_char;

    struct car res_qualif[len_initial_cars];
    struct car res_free_practice[len_initial_cars];
    struct car res_race[len_initial_cars];
    struct best_time fastest_lap[1];

    // P1
    printf("Would you like to start the first free practice session (P1)? (yes/no)\n");
    scanf("%3s", user_decision);
    while ((flush_char = getchar()) != '\n' && flush_char != EOF);

    if (strcmp(user_decision, "yes") == 0){
        run_a_qualif(initial_cars, len_initial_cars, time_free_practice, res_free_practice);
    }

    // P2
    printf("Would you like to start the second free practice session (P2)? (yes/no)\n");
    scanf("%3s", user_decision);
    while ((flush_char = getchar()) != '\n' && flush_char != EOF);

    if (strcmp(user_decision, "yes") == 0){
        run_a_qualif(initial_cars, len_initial_cars, time_free_practice, res_free_practice);
    }

    // P3
    printf("Would you like to start the third free practice session (P3)? (yes/no)\n");
    scanf("%3s", user_decision);
    while ((flush_char = getchar()) != '\n' && flush_char != EOF);

    if (strcmp(user_decision, "yes") == 0){
        run_a_qualif(initial_cars, len_initial_cars, time_free_practice, res_free_practice);
    }

    // Qualifications
    printf("Would you like to start the qualifications? (yes/no)\n");
    scanf("%3s", user_decision);
    while ((flush_char = getchar()) != '\n' && flush_char != EOF);

    if (strcmp(user_decision, "yes") == 0){
        printf("\nStart of the first qualification (Q1)\n");
        sleep(show_delay);
        run_a_qualif(initial_cars, len_initial_cars, q1_classic, res_qualif);

        printf("Eliminated cars: ");
        for (int i = 0; i < len_initial_cars; i++) {
            if (i >= 15 && i < 19){
                grid_race[i] = res_qualif[i].number;
                printf("%d, ", res_qualif[i].number);
            }
            else if (i == 19){
                grid_race[i] = res_qualif[i].number;
                printf("%d\n", res_qualif[i].number);
            } else {
                cars_q2[i] = res_qualif[i].number;
            }
        }

        printf("\nStart of the second qualification (Q2)\n");
        sleep(show_delay);
        run_a_qualif(cars_q2, len_cars_q2, q2_classic, res_qualif);

        printf("Eliminated cars: ");
        for (int i = 0; i < 15; i++) {
            if (i >= 10 && i < 14){
                grid_race[i] = res_qualif[i].number;
                printf("%d, ", res_qualif[i].number);
            }
            else if (i == 14){
                grid_race[i] = res_qualif[i].number;
                printf("%d\n", res_qualif[i].number);
            } else {
                cars_q3[i] = res_qualif[i].number;
            }
        }

        printf("\nStart of the third qualification (Q3)\n");
        sleep(show_delay);
        run_a_qualif(cars_q3, len_cars_q3, q3_classic, res_qualif);

        for (int i = 0; i < 10; i++) {
            grid_race[i] = res_qualif[i].number;
        }

        printf("Car %d takes pole position!\n\n", grid_race[0]);
        printf("---------------------------\n");
        printf("  MAIN RACE STARTING GRID\n");
        printf("---------------------------\n\n");
        starting_grid_format(grid_race, len_initial_cars);
    } else {
        for (int i = 0; i < len_initial_cars; i++) {
            grid_race[i] = initial_cars[i];
        }
    }

    // Course
    printf("Would you like to start the race? (yes/no)\n");
    scanf("%3s", user_decision);
    while ((flush_char = getchar()) != '\n' && flush_char != EOF);

    if (strcmp(user_decision, "yes") == 0){
        printf("\nStart of the race\n");
        sleep(show_delay);

        run_a_race(grid_race, len_initial_cars, max_lap, res_race, fastest_lap);
        printf("Car %d wins the race!\n\n", res_race[0].number);

        for(int idx_drv=0; idx_drv<len_initial_cars; idx_drv++){
            for(int idx_res=0; idx_res<10; idx_res++){
                if(all_drivers[idx_drv].number == res_race[idx_res].number){
                    all_drivers[idx_drv].points += points_awarded[idx_res];
                    if(all_drivers[idx_drv].number == fastest_lap[0].car_number){
                        all_drivers[idx_drv].points += 1;
                        printf("Car %d takes the bonus point for the fastest lap!\n\n", fastest_lap[0].car_number);
                    }
                }
            }
        }
    }

    // Classement
    printf("Would you like to see the championship standings? (yes/no)\n");
    scanf("%3s", user_decision);
    while ((flush_char = getchar()) != '\n' && flush_char != EOF);

    if (strcmp(user_decision, "yes") == 0){
        system("clear");
        printf("\n2023 FIA Formula 1 World Championship standings:\n\n");
        championship_format(all_drivers, len_initial_cars);
    }
}

void weekend_special(struct drivers all_drivers[20], int show_delay, int max_lap_race, int max_lap_sprint){
    int initial_cars[20] = {1, 11, 16, 55, 63, 44, 31, 10, 4, 81,
                            77, 24, 14, 18, 20, 27, 22, 3, 23, 2};
    int len_cars = sizeof(initial_cars) / sizeof(initial_cars[0]);

    int cars_q2[15];
    int len_q2 = sizeof(cars_q2) / sizeof(cars_q2[0]);
    int cars_q3[10];
    int len_q3 = sizeof(cars_q3) / sizeof(cars_q3[0]);

    int grid_race[20];
    int grid_sprint[20];

    int points_classic[10] = {25, 20, 15, 10, 8, 6, 5, 3, 2, 1};
    int points_sprint[8] = {8, 7, 6, 5, 4, 3, 2, 1};

    int free_practice_sprint = 5;
    int q1_cls = 9;
    int q2_cls = 7;
    int q3_cls = 5;

    int q1_spr = 6;
    int q2_spr = 5;
    int q3_spr = 4;

    char user_decision[4];
    int flush_char;

    struct car res_qualif[len_cars];
    struct car res_free[len_cars];
    struct car res_race[len_cars];
    struct car res_sprint[len_cars];
    struct best_time best_lap_classic[1];
    struct best_time best_lap_sprint[1];

    // Free practice (P1)
    printf("Would you like to start the free practice session (P1)? (yes/no)\n");
    scanf("%3s", user_decision);
    while ((flush_char = getchar()) != '\n' && flush_char != EOF);

    if (strcmp(user_decision, "yes") == 0){
        run_a_qualif(initial_cars, len_cars, free_practice_sprint, res_free);
    }

    /******************************************************/
    /******************      CLASSIC      *****************/
    /******************************************************/
    printf("Would you like to start the qualifications? (yes/no)\n");
    scanf("%3s", user_decision);
    while ((flush_char = getchar()) != '\n' && flush_char != EOF);

    if (strcmp(user_decision, "yes") == 0){
        printf("\nStart of the first qualification (Q1)\n");
        sleep(show_delay);
        run_a_qualif(initial_cars, len_cars, q1_cls, res_qualif);

        printf("Eliminated cars: ");
        for (int i = 0; i < len_cars; i++) {
            if (i >= 15 && i < 19){
                grid_race[i] = res_qualif[i].number;
                printf("%d, ", res_qualif[i].number);
            }
            else if (i == 19){
                grid_race[i] = res_qualif[i].number;
                printf("%d\n", res_qualif[i].number);
            } else {
                cars_q2[i] = res_qualif[i].number;
            }
        }

        printf("\nStart of the second qualification (Q2)\n");
        sleep(show_delay);
        run_a_qualif(cars_q2, len_q2, q2_cls, res_qualif);

        printf("Eliminated cars: ");
        for (int i = 0; i < 15; i++) {
            if (i >= 10 && i < 14){
                grid_race[i] = res_qualif[i].number;
                printf("%d, ", res_qualif[i].number);
            }
            else if (i == 14){
                grid_race[i] = res_qualif[i].number;
                printf("%d\n", res_qualif[i].number);
            } else {
                cars_q3[i] = res_qualif[i].number;
            }
        }

        printf("\nStart of the third qualification (Q3)\n");
        sleep(show_delay);
        run_a_qualif(cars_q3, len_q3, q3_cls, res_qualif);

        for (int i = 0; i < 10; i++) {
            grid_race[i] = res_qualif[i].number;
        }
        printf("Car %d takes pole position!\n\n", grid_race[0]);
        printf("---------------------------\n");
        printf("  MAIN RACE STARTING GRID\n");
        printf("---------------------------\n\n");
        starting_grid_format(grid_race, len_cars);
    } else {
        for (int i = 0; i < len_cars; i++) {
            grid_race[i] = initial_cars[i];
        }
    }

    /******************************************************/
    /******************      SPRINT       *****************/
    /******************************************************/
    printf("Would you like to start the sprint qualifications? (yes/no)\n");
    scanf("%3s", user_decision);
    while ((flush_char = getchar()) != '\n' && flush_char != EOF);

    if (strcmp(user_decision, "yes") == 0){
        printf("\nStart of the first qualification (Q1)\n");
        sleep(show_delay);
        run_a_qualif(initial_cars, len_cars, q1_spr, res_qualif);

        printf("Eliminated cars: ");
        for (int i = 0; i < len_cars; i++) {
            if (i >= 15 && i < 19){
                grid_sprint[i] = res_qualif[i].number;
                printf("%d, ", res_qualif[i].number);
            }
            else if (i == 19){
                grid_sprint[i] = res_qualif[i].number;
                printf("%d\n", res_qualif[i].number);
            } else {
                cars_q2[i] = res_qualif[i].number;
            }
        }

        printf("\nStart of the second qualification (Q2)\n");
        sleep(show_delay);
        run_a_qualif(cars_q2, len_q2, q2_spr, res_qualif);

        printf("Eliminated cars: ");
        for (int i = 0; i < 15; i++) {
            if (i >= 10 && i < 14){
                grid_sprint[i] = res_qualif[i].number;
                printf("%d, ", res_qualif[i].number);
            }
            else if (i == 14){
                grid_sprint[i] = res_qualif[i].number;
                printf("%d\n", res_qualif[i].number);
            } else {
                cars_q3[i] = res_qualif[i].number;
            }
        }

        printf("\nStart of the third qualification (Q3)\n");
        sleep(show_delay);
        run_a_qualif(cars_q3, len_q3, q3_spr, res_qualif);

        for (int i = 0; i < 10; i++) {
            grid_sprint[i] = res_qualif[i].number;
        }
        printf("Car %d takes pole position!\n\n", grid_sprint[0]);
        printf("---------------------------\n");
        printf("  SPRINT RACE STARTING GRID\n");
        printf("---------------------------\n\n");
        starting_grid_format(grid_sprint, len_cars);
    } else {
        for (int i = 0; i < len_cars; i++) {
            grid_sprint[i] = initial_cars[i];
        }
    }

    printf("Would you like to start the sprint race? (yes/no)\n");
    scanf("%3s", user_decision);
    while ((flush_char = getchar()) != '\n' && flush_char != EOF);

    if (strcmp(user_decision, "yes") == 0){
        sleep(show_delay);
        printf("\nStart of the sprint race\n");

        run_a_race(grid_sprint, len_cars, max_lap_sprint, res_sprint, best_lap_sprint);
        printf("Car %d wins the sprint race!\n\n", res_sprint[0].number);

        for(int idx_drv=0; idx_drv<len_cars; idx_drv++){
            for(int idx_res=0; idx_res<8; idx_res++){
                if(all_drivers[idx_drv].number == res_sprint[idx_res].number){
                    all_drivers[idx_drv].points += points_sprint[idx_res];
                }
            }
        }
    }

    printf("Would you like to see the championship standings? (yes/no)\n");
    scanf("%3s", user_decision);
    while ((flush_char = getchar()) != '\n' && flush_char != EOF);

    if (strcmp(user_decision, "yes") == 0){
        system("clear");
        printf("\n2023 FIA Formula 1 World Championship standings:\n\n");
        championship_format(all_drivers, len_cars);
    }

    /******************************************************/
    /******************       RACE        *****************/
    /******************************************************/
    printf("\nWould you like to start the race? (yes/no)\n");
    scanf("%3s", user_decision);
    while ((flush_char = getchar()) != '\n' && flush_char != EOF);

    if (strcmp(user_decision, "yes") == 0){
        printf("\nStart of the race\n");
        sleep(show_delay);

        run_a_race(grid_race, len_cars, max_lap_race, res_race, best_lap_classic);
        printf("Car %d wins the race!\n\n", res_race[0].number);

        for(int idx_drv=0; idx_drv<len_cars; idx_drv++){
            for(int idx_res=0; idx_res<10; idx_res++){
                if(all_drivers[idx_drv].number == res_race[idx_res].number){
                    all_drivers[idx_drv].points += points_classic[idx_res];
                    if(all_drivers[idx_drv].number == best_lap_classic[0].car_number){
                        all_drivers[idx_drv].points += 1;
                        printf("Car %d takes the bonus point for the fastest lap!\n\n", best_lap_classic[0].car_number);
                    }
                }
            }
        }
    }

    printf("Would you like to see the championship standings? (yes/no)\n");
    scanf("%3s", user_decision);
    while ((flush_char = getchar()) != '\n' && flush_char != EOF);

    if (strcmp(user_decision, "yes") == 0){
        system("clear");
        printf("\n2023 FIA Formula 1 World Championship standings:\n\n");
        championship_format(all_drivers, len_cars);
    }
}
