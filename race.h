#include <stdlib.h>
#include <stdio.h>
#include <time.h> 
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/types.h>      /* key_t, sem_t, pid_t      */
#include <sys/shm.h>        /* shmat(), IPC_RMID        */
#include <errno.h>          /* errno, ECHILD            */
#include <semaphore.h>      /* sem_open(), sem_post(), sem_wait().. */
#include <fcntl.h>          /* O_CREAT, O_EXEC          */
#include <sys/wait.h>       /* waitpid()                */
#include "format.h"

void run_a_race(int list_cars[], int length, int max_lap, struct car qualifierd_cars[], struct best_time fastest_lap[1]) {
    int shared_cars_id = shmget(2, sizeof(car) * length, IPC_CREAT | 0666);
    struct car *shared_cars  = shmat(shared_cars_id, NULL, 0);
    if (shared_cars_id < 0){
        perror("shmget not working for shared_cars\n"); //shared memory error check for shared_cars
        exit(-1);
    }

    int best_time_id = shmget(3, sizeof(best_time) * 4, IPC_CREAT | 0666);
    struct best_time *all_best_time = shmat(best_time_id, NULL, 0);
    if (best_time_id < 0){
        perror("shmget not working for best_time\n"); //shared memory error check for best_time
        exit(-1);
    }

    for (int best_time_init=0; best_time_init < 4; best_time_init++) {
        all_best_time[best_time_init] = default_best;
    }

    int fork_id;
    int current_car;
    int current_lap;
    struct car sorted_cars[length];

    /******************************************************/
    /******************     MAKE CARS      ****************/
    /******************************************************/
    for (current_lap = 0; current_lap < max_lap; current_lap++) {
        for (current_car = 0; current_car < length; current_car++) {
            fork_id = fork();
            if (fork_id < 0) {
                printf("Fork error.\n");
            } else if (fork_id == 0) {
                break;
            }
        }
        /******************************************************/
        /******************   PARENT PROCESS   ****************/
        /******************************************************/
        if (fork_id > 0) {
            /* wait for all children to exit */
            while (fork_id = waitpid(-1, NULL, 0)) {
                if (errno == ECHILD)
                    break;
            }

            for (current_car = 0; current_car < length; current_car++) {
                sorted_cars[current_car] = shared_cars[current_car];
            }
            cars_sort_race(sorted_cars, length);
            system("clear");
            printf("  -------------\n");
            printf("  | Lap %2d/%2d |\n", current_lap+1, max_lap);
            printf("  -------------\n");
            race_time_format(sorted_cars, all_best_time, length);
        }
            /******************************************************/
            /******************   CHILD PROCESS   *****************/
            /******************************************************/
        else {
            if (current_lap == 0) {
                shared_cars[current_car] = generate_car(list_cars[current_car]);

            }

            struct car car = shared_cars[current_car];
            if (!car.has_crashed) {
                srand(time(NULL) ^ getpid());
                int crash_luck = rand() % 1000;
                if (crash_luck < 1) {
                    car.tt = 9999999;
                    car.has_crashed = true;
                } else {
                    run_a_lap(&car, all_best_time);
                }
                shared_cars[current_car] = car;
            }

            exit(0);
        }
        sleep(1);
    }
    for(int i = 0; i < length; i++) { // copy the cars in order in qualifierd_cars
        qualifierd_cars[i] = sorted_cars[i];
    }
    fastest_lap[0] = all_best_time[3];
    // detach shared memory
    shmdt(shared_cars);
    shmctl (shared_cars_id, IPC_RMID, 0);
    shmdt(all_best_time);
    shmctl (best_time_id, IPC_RMID, 0);
}

void run_a_qualif(int list_cars[], int length, int TOTAL_TIME, struct car qualified_cars[]) {
    int shared_cars_id = shmget(2, sizeof(car) * length, IPC_CREAT | 0666);
    struct car *shared_cars  = shmat(shared_cars_id, NULL, 0);
    if (shared_cars_id < 0){
        perror("shmget not working for shared_cars\n"); //shared memory error check for shared_cars
        exit(-1);
    }

    int best_time_id = shmget(3, sizeof(best_time) * 4, IPC_CREAT | 0666);
    struct best_time *all_best_time = shmat(best_time_id, NULL, 0);
    if (best_time_id < 0){
        perror("shmget not working for best_time\n"); //shared memory error check for best_time
        exit(-1);
    }

    int sem_reader_id = shmget (4, sizeof (int), 0644 | IPC_CREAT);
    sem_t *sem_reader;
    int *sem_reader_value;
    if (sem_reader_id < 0){
        perror("shmget not working for sem_reader\n"); //shared memory error check for sem_reader
        exit(-1);
    }
    sem_reader_value = (int *) shmat (sem_reader_id, NULL, 0);   // attach sem_reader_value to shared memory
    *sem_reader_value = 0;
    // initialize semaphores for shared processes
    sem_reader = sem_open("p_sem_reader", O_CREAT | O_EXCL, 0644, 1);
    // name of semaphore is "p_sem_reader", semaphore is reached using this name

    int sem_writer_id = shmget (5, sizeof (int), 0644 | IPC_CREAT);
    sem_t *sem_writer;
    int *sem_writer_value;
    if (sem_writer_id < 0){
        perror("shmget not working for sem_writer\n"); //shared memory error check for sem_writer
        exit(-1);
    }
    sem_writer_value = (int *) shmat (sem_writer_id, NULL, 0);   // attach sem_writer_value to shared memory
    *sem_writer_value = 0;
    // initialize semaphores for shared processes
    sem_writer = sem_open("p_sem_writer", O_CREAT | O_EXCL, 0644, length);
    // name of semaphore is "p_sem_writer", semaphore is reached using this name

    for (int best_time_init=0; best_time_init < 4; best_time_init++) {
        all_best_time[best_time_init] = default_best;
    }

	int current_car;
	int fork_id;
    int trigger = 1;
    double elapsed_time = 0;
    time_t before, after;
    time(&before);

    /******************************************************/
    /******************     MAKE CARS      ****************/
    /******************************************************/
	for (current_car=0; current_car < length; current_car++) {
        shared_cars[current_car] = generate_car(list_cars[current_car]); // init les cars
		fork_id = fork();
		if (fork_id < 0) {
            sem_unlink ("p_sem_reader"); // unlink prevents the semaphore existing forever if a crash occurs during the execution
            sem_close(sem_reader);
            sem_unlink ("p_sem_writer");
            sem_close(sem_writer);
            printf("Fork error.\n");
        } else if (fork_id == 0) {
            break;
		}
	}
    /******************************************************/
    /******************   PARENT PROCESS   ****************/
    /******************************************************/
    if (fork_id > 0) {
        int current_number_writers;
        struct car sorted_cars[length];
        while(elapsed_time < TOTAL_TIME) {
            sleep(trigger);

            sem_wait(sem_reader); // < S.C > start - reading shared memory
            sem_getvalue(sem_writer, &current_number_writers); //check if a child is writing
            while (current_number_writers != length){
                sem_getvalue(sem_writer, &current_number_writers);
            }
            for (current_car=0; current_car < length; current_car++) {
                sorted_cars[current_car] = shared_cars[current_car];
            }
            cars_sort_qualif(sorted_cars, length);
            system("clear");
            qualif_time_format(sorted_cars, all_best_time, length);

            sem_post(sem_reader); // < S.C > end

            time(&after);
            elapsed_time = difftime(after, before);
        }
        for(int i = 0; i < length; i++) { // copy the cars in order in qualified_cars
            qualified_cars[i] = sorted_cars[i];
        }
    }
    /******************************************************/
    /******************   CHILD PROCESS   *****************/
    /******************************************************/
    else {
        int current_number_readers;

        struct car car = shared_cars[current_car];
        srand(time(NULL) ^ getpid());

        while (elapsed_time < TOTAL_TIME){
            sem_getvalue(sem_reader, &current_number_readers); //check if parent is reading
            while (current_number_readers < 1){
                sem_getvalue(sem_writer, &current_number_readers);
                time(&after);
                elapsed_time = difftime(after, before);
                if (elapsed_time >= TOTAL_TIME){ // if child is waiting for parent and time is up, exit
                    break;
                }
            }

            sem_wait(sem_writer); // < S.C > start - writing in shared memory
            run_a_lap(&car, all_best_time);
            shared_cars[current_car] = car;

            sem_post(sem_writer); // < S.C > end

            sleep(trigger);
            time(&after);
            elapsed_time = difftime(after, before);
        }
        exit(0);
    }

    // detach shared memory
    shmdt(shared_cars);
    shmctl (shared_cars_id, IPC_RMID, 0);
    shmdt(all_best_time);
    shmctl (best_time_id, IPC_RMID, 0);

    shmdt (sem_reader_value);
    shmctl (sem_reader_id, IPC_RMID, 0);
    shmdt (sem_writer_value);
    shmctl (sem_writer_id, IPC_RMID, 0);

    // cleanup semaphores
    sem_unlink ("p_sem_reader"); // unlink prevents the semaphore existing forever if a crash occurs during the execution
    sem_close(sem_reader);
    sem_unlink ("p_sem_writer");
    sem_close(sem_writer);
}