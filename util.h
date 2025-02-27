#include <stdlib.h>
#include <stdio.h>
#include <time.h> 
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <semaphore.h>
#include "struct.h"

// --- TIME ---

int get_car_time(){
    int MIN_TIME = 25000;
    int MAX_TIME = 45000;
    return rand() % (MAX_TIME - MIN_TIME + 1) + MIN_TIME;
}

// --- CAR ---

const int DEFAULT = -1;
struct best_time default_best = {DEFAULT, DEFAULT};

struct car generate_car(int number) {
    struct car new_car = {number, DEFAULT, DEFAULT, DEFAULT, DEFAULT, DEFAULT, false};
    return new_car;
}

void swap(struct car *xp, struct car *yp){
    struct car temp = *xp; 
    *xp = *yp; 
    *yp = temp; 
}

void swap_drivers(struct drivers *xp, struct drivers *yp){
    struct drivers tmp_dv = *xp;
    *xp = *yp;
    *yp = tmp_dv;
}

void cars_sort_qualif(struct car arr[], int n){
    int i, j, min_idx;
    for (i = 0; i < n - 1; i++){
        min_idx = i;
        for (j = i + 1; j < n; j++){
            if (arr[j].bt < arr[min_idx].bt){
                min_idx = j;
            }
        }
        swap(&arr[min_idx], &arr[i]);
    }
}

void cars_sort_race(struct car arr[], int n){
    int i, j, min_idx;
    for (i = 0; i < n - 1; i++){
        min_idx = i;
        for (j = i + 1; j < n; j++){
            if (arr[j].tt < arr[min_idx].tt){
                min_idx = j;
            }
        }
        swap(&arr[min_idx], &arr[i]);
    }
}

void drivers_sort(struct drivers arr[], int n){
    int i, j, max_idx;
    for (i = 0; i < n - 1; i++){
        max_idx = i;
        for (j = i + 1; j < n; j++){
            if (arr[j].points > arr[max_idx].points){
                max_idx = j;
            }
        }
        swap_drivers(&arr[max_idx], &arr[i]);
    }
}

void run_a_lap(struct car *current_car, struct best_time *list_best_times){
    current_car->s1 = get_car_time();
    current_car->s2 = get_car_time();
    current_car->s3 = get_car_time();

    int current_lap_time = current_car->s1 + current_car->s2 + current_car->s3;
    current_car->tt += current_lap_time;

    if ((current_car->bt == DEFAULT) || (current_car->bt > current_lap_time)) {
        current_car->bt = current_lap_time;
    }

    // Mise à jour des meilleurs secteurs
    if ((list_best_times[0].time == DEFAULT) || (list_best_times[0].time > current_car->s1)) {
        list_best_times[0].time = current_car->s1;
        list_best_times[0].car_number = current_car->number;
    }
    if ((list_best_times[1].time == DEFAULT) || (list_best_times[1].time > current_car->s2)) {
        list_best_times[1].time = current_car->s2;
        list_best_times[1].car_number = current_car->number;
    }
    if ((list_best_times[2].time == DEFAULT) || (list_best_times[2].time > current_car->s3)) {
        list_best_times[2].time = current_car->s3;
        list_best_times[2].car_number = current_car->number;
    }
    // Mise à jour du meilleur tour
    if ((list_best_times[3].time == DEFAULT) || (list_best_times[3].time > current_car->bt)) {
        list_best_times[3].time = current_car->bt;
        list_best_times[3].car_number = current_car->number;
    }
}
