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

	int car_time;
	car_time = rand() % (MAX_TIME - MIN_TIME + 1) + MIN_TIME;

	return car_time;
}

// --- CAR ---

const int DEFAULT = -1;

struct best_time default_best = {DEFAULT, DEFAULT};

struct car generate_car(int number) {
	struct car car = {number, DEFAULT, DEFAULT, DEFAULT, DEFAULT, DEFAULT, false};
	return car;
};

void swap(struct car * xp, struct car * yp) 
{ 
    struct car  temp = *xp; 
    *xp = *yp; 
    *yp = temp; 
} 

void swap_drivers(struct drivers * xp, struct drivers * yp) 
{ 
    struct drivers temp = *xp; 
    *xp = *yp; 
    *yp = temp; 
} 

void cars_sort_qualif(struct car arr[], int n) 
{ 
    int i, j, min_idx; 
  
    for (i = 0; i < n - 1; i++) { 
        min_idx = i; 
        for (j = i + 1; j < n; j++) 
            if (arr[j].bt < arr[min_idx].bt) 
                min_idx = j; 
  
        swap(&arr[min_idx], &arr[i]); 
    } 
} 

void cars_sort_race(struct car arr[], int n) 
{ 
    int i, j, min_idx; 
  
    for (i = 0; i < n - 1; i++) { 
        min_idx = i; 
        for (j = i + 1; j < n; j++) 
            if (arr[j].tt < arr[min_idx].tt) 
                min_idx = j; 
  
        swap(&arr[min_idx], &arr[i]); 
    } 
} 

void drivers_sort(struct drivers arr[], int n){

    int i, j, min_idx; 
  
    for (i = 0; i < n - 1; i++) { 
        min_idx = i; 
        for (j = i + 1; j < n; j++) 
            if (arr[j].points > arr[min_idx].points) 
                min_idx = j; 
  
        swap_drivers(&arr[min_idx], &arr[i]); 
    } 
}

void run_a_lap(struct car *car, struct best_time *list_best_times) {
	car->s1 = get_car_time();
	car->s2 = get_car_time();
	car->s3 = get_car_time();

	int lap_time = car->s1 + car->s2 + car->s3;
	car->tt += lap_time;

	if ((car->bt == DEFAULT) || (car->bt > lap_time)) {
		car->bt = lap_time;
	}

	if ((list_best_times[0].time == DEFAULT) || (list_best_times[0].time > car->s1)) {
        list_best_times[0].time = car->s1;
        list_best_times[0].car_number = car->number;
	}

	if ((list_best_times[1].time == DEFAULT) || (list_best_times[1].time > car->s2)) {
        list_best_times[1].time = car->s2;
        list_best_times[1].car_number = car->number;
	}

	if ((list_best_times[2].time == DEFAULT) || (list_best_times[2].time > car->s3)) {
        list_best_times[2].time = car->s3;
        list_best_times[2].car_number = car->number;
	}

	if ((list_best_times[3].time == DEFAULT) || (list_best_times[3].time > car->bt)) {
        list_best_times[3].time = car->bt;
        list_best_times[3].car_number = car->number;
	}
}
