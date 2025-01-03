#include <stdlib.h>
#include <stdio.h>
#include <time.h> 
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <semaphore.h>
#include "gp_struct.h"

// --- TIME ---
int getRandomCarTime() {
    int MIN_TIME = 25000;
    int MAX_TIME = 45000;
    return rand() % (MAX_TIME - MIN_TIME + 1) + MIN_TIME;
}

// --- CAR ---
const int DEFAULT = -1;
struct best_time default_best = {DEFAULT, DEFAULT};

struct car generate_car(int number) {
    struct car newCar = {number, DEFAULT, DEFAULT, DEFAULT, DEFAULT, DEFAULT, false};
    return newCar;
}

void swapCar(struct car *x, struct car *y) {
    struct car tmp = *x; 
    *x = *y; 
    *y = tmp; 
}

void swapDriver(struct drivers *x, struct drivers *y) {
    struct drivers tmp = *x; 
    *x = *y; 
    *y = tmp; 
}

// Tri qualif (par BT)
void cars_sort_qualif(struct car arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j].bt < arr[min_idx].bt)
                min_idx = j;
        }
        swapCar(&arr[min_idx], &arr[i]);
    }
}

// Tri course (par TT)
void cars_sort_race(struct car arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j].tt < arr[min_idx].tt)
                min_idx = j;
        }
        swapCar(&arr[min_idx], &arr[i]);
    }
}

// Tri coureurs (par points)
void drivers_sort(struct drivers arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int max_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j].points > arr[max_idx].points)
                max_idx = j;
        }
        swapDriver(&arr[max_idx], &arr[i]);
    }
}

void run_a_lap(struct car *pCar, struct best_time *bestList) {
    pCar->s1 = getRandomCarTime();
    pCar->s2 = getRandomCarTime();
    pCar->s3 = getRandomCarTime();

    int lap_time = pCar->s1 + pCar->s2 + pCar->s3;
    pCar->tt += lap_time;

    if (pCar->bt == DEFAULT || pCar->bt > lap_time) {
        pCar->bt = lap_time;
    }

    // Mettre à jour meilleurs secteurs
    if (bestList[0].time == DEFAULT || bestList[0].time > pCar->s1) {
        bestList[0].time = pCar->s1;
        bestList[0].car_number = pCar->number;
    }
    if (bestList[1].time == DEFAULT || bestList[1].time > pCar->s2) {
        bestList[1].time = pCar->s2;
        bestList[1].car_number = pCar->number;
    }
    if (bestList[2].time == DEFAULT || bestList[2].time > pCar->s3) {
        bestList[2].time = pCar->s3;
        bestList[2].car_number = pCar->number;
    }
    // Meilleur tour
    if (bestList[3].time == DEFAULT || bestList[3].time > pCar->bt) {
        bestList[3].time = pCar->bt;
        bestList[3].car_number = pCar->number;
    }
}
