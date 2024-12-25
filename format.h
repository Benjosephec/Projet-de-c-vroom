#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "util.h"

void lap_time_format(int lap_time){
    int min = lap_time / (60 * 1000);
    int s = (lap_time % (60 * 1000)) / 1000;
    int ms = lap_time % 1000;

    printf("  |  %d:%02d.%03d", min, s, ms);
}

void sector_time_format(int sector_time){
    int s = (sector_time % (60 * 1000)) / 1000;
    int ms = sector_time % 1000;

    printf("  |  %02d.%03d", s, ms);
}

void gap_time_format(int gap_time){
    int s = (gap_time % (60 * 1000)) / 1000;
    int ms = gap_time % 1000;

    printf("  |  +%d.%03d", s, ms);
}

void qualif_time_format(struct car sorted_cars[], struct best_time best_times[], int SIZE){
    int i;
    printf("\n  Car  |  Lap Time  |   Gap  \n");
    printf("--------------------------------\n");
    for (i=0; i < SIZE; i++) {
        struct car car = sorted_cars[i];

        int gap = 0;

        printf("   %2d", car.number);
        lap_time_format(car.bt);
        if (i!= 0) {
            gap = car.bt - sorted_cars[i-1].bt;
            gap_time_format(gap);
        } else {
            printf("  |  LEADER");
        }
        printf("\n");
    }
    struct best_time best_s1 = best_times[0];
    struct best_time best_s2 = best_times[1];
    struct best_time best_s3 = best_times[2];
    struct best_time best_bt = best_times[3];

    printf("\nBest Times :\n\n");
    printf("        |    S1    |    S2    |    S3    |    Lap\n");
    printf("------------------------------------------------------\n");


    printf("   Car  |    %2d    |    %2d    |    %2d    |    %2d\n", best_s1.car_number, best_s2.car_number, best_s3.car_number, best_bt.car_number);
    printf("------------------------------------------------------\n");
    printf("  Time");

    sector_time_format(best_s1.time);
    sector_time_format(best_s2.time);
    sector_time_format(best_s3.time);
    lap_time_format(best_bt.time);
    printf("\n\n");
}

void race_time_format(struct car sorted_cars[], struct best_time best_times[], int SIZE){
    int i;
    printf("\n  Car  |   Gap\n");
    printf("------------------\n");
    for (i=0; i < SIZE; i++) {
        struct car car = sorted_cars[i];

        printf("   %2d", car.number);

        if (car.has_crashed) {
            printf("  |  OUT");
        }
        else {
            if (i != 0) {
                int gap;
                gap = car.tt - sorted_cars[i - 1].tt;
                gap_time_format(gap);
            } else {
                printf("  |  LEADER");
            }
        }
        printf("\n");
    }
    struct best_time best_s1 = best_times[0];
    struct best_time best_s2 = best_times[1];
    struct best_time best_s3 = best_times[2];
    struct best_time best_bt = best_times[3];

    printf("\nBest Times :\n\n");
    printf("        |    S1    |    S2    |    S3    |    Lap\n");
    printf("------------------------------------------------------\n");


    printf("   Car  |    %2d    |    %2d    |    %2d    |    %2d\n", best_s1.car_number, best_s2.car_number, best_s3.car_number, best_bt.car_number);
    printf("------------------------------------------------------\n");
    printf("  Time");

    sector_time_format(best_s1.time);
    sector_time_format(best_s2.time);
    sector_time_format(best_s3.time);
    lap_time_format(best_bt.time);
    printf("\n\n");
}

void championship_format(struct drivers all_drivers[], int num_drivers){
    drivers_sort(all_drivers, num_drivers);

    printf("  Car  |    Driver    |  Points\n");
    printf("---------------------------------\n");
    for(int i=0; i<num_drivers; i++){
        printf("   %2d  |  %10s  |    %2d\n", all_drivers[i].number, all_drivers[i].last_name, all_drivers[i].points);
    }
}

void starting_grid_format(int sort_cars[], int num_cars){
    printf("  #  |  Car\n");
    printf("-------------\n");
    for (int i=0; i<num_cars; i++){
        printf(" %2d  |   %2d\n", i+1, sort_cars[i]);
    }
    printf("\n");
}
