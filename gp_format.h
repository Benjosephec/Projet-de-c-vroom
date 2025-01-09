#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "gp_util.h"

void displayLapTime(int lapTime) {
    int min  = lapTime / (60 * 1000);
    int sec  = (lapTime % (60 * 1000)) / 1000;
    int mili = lapTime % 1000;
    printf("  |  %d:%02d.%03d", min, sec, mili);
}

void displaySectorTime(int sectorTime) {
    int sec  = (sectorTime % (60 * 1000)) / 1000;
    int mili = sectorTime % 1000;
    printf("  |  %02d.%03d", sec, mili);
}

void displayGapTime(int gap) {
    int sec  = (gap % (60 * 1000)) / 1000;
    int mili = gap % 1000;
    printf("  |  +%d.%03d", sec, mili);
}

void showQualifTimes(struct car sortedCars[], struct best_time bestTimes[], int size) {
    printf("\n  Car  |  Lap Time  |   Gap  \n");
    printf("--------------------------------\n");
    for (int i = 0; i < size; i++) {
        struct car tmpCar = sortedCars[i];
        int gap = 0;

        printf("   %2d", tmpCar.number);
        displayLapTime(tmpCar.bt);
        if (i != 0) {
            gap = tmpCar.bt - sortedCars[i - 1].bt;
            displayGapTime(gap);
        } else {
            printf("  |  LEADER");
        }
        printf("\n");
    }

    struct best_time best_s1 = bestTimes[0];
    struct best_time best_s2 = bestTimes[1];
    struct best_time best_s3 = bestTimes[2];
    struct best_time best_bt = bestTimes[3];

    printf("\nBest Times:\n\n");
    printf("        |    S1    |    S2    |    S3    |    Lap\n");
    printf("------------------------------------------------------\n");
    printf("   Car  |    %2d    |    %2d    |    %2d    |    %2d\n",
        best_s1.car_number, best_s2.car_number, best_s3.car_number, best_bt.car_number);
    printf("------------------------------------------------------\n");
    printf("  Time");
    displaySectorTime(best_s1.time);
    displaySectorTime(best_s2.time);
    displaySectorTime(best_s3.time);
    displayLapTime(best_bt.time);
    printf("\n\n");
}

void showRaceTimes(struct car sortedCars[], struct best_time bestTimes[], int size) {
    printf("\n  Car  |   Gap\n");
    printf("------------------\n");
    for (int i = 0; i < size; i++) {
        struct car tmpCar = sortedCars[i];

        printf("   %2d", tmpCar.number);
        if (tmpCar.has_crashed) {
            printf("  |  OUT");
        } else {
            if (i != 0) {
                int gap = tmpCar.tt - sortedCars[i - 1].tt;
                displayGapTime(gap);
            } else {
                printf("  |  LEADER");
            }
        }
        printf("\n");
    }

    struct best_time best_s1 = bestTimes[0];
    struct best_time best_s2 = bestTimes[1];
    struct best_time best_s3 = bestTimes[2];
    struct best_time best_bt = bestTimes[3];

    printf("\nBest Times:\n\n");
    printf("        |    S1    |    S2    |    S3    |    Lap\n");
    printf("------------------------------------------------------\n");
    printf("   Car  |    %2d    |    %2d    |    %2d    |    %2d\n",
        best_s1.car_number, best_s2.car_number, best_s3.car_number, best_bt.car_number);
    printf("------------------------------------------------------\n");
    printf("  Time");
    displaySectorTime(best_s1.time);
    displaySectorTime(best_s2.time);
    displaySectorTime(best_s3.time);
    displayLapTime(best_bt.time);
    printf("\n\n");
}

void showChampionshipStandings(struct drivers allDrivers[], int nbDrivers) {
    drivers_sort(allDrivers, nbDrivers);
    printf("  Car  |    Driver    |  Points\n");
    printf("---------------------------------\n");
    for (int i = 0; i < nbDrivers; i++) {
        printf("   %2d  |  %10s  |    %2d\n",
               allDrivers[i].number, allDrivers[i].last_name, allDrivers[i].points);
    }
}

void showStartingGrid(int sortedCars[], int nbCars) {
    printf("  #  |  Car\n");
    printf("-------------\n");
    for (int i = 0; i < nbCars; i++) {
        printf(" %2d  |   %2d\n", i + 1, sortedCars[i]);
    }
    printf("\n");
}
