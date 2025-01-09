#include <stdlib.h>
#include <stdio.h>
#include <time.h> 
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <errno.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "format.h"

void run_a_race(int list_cars[], int length, int max_lap, struct car qualifierd_cars[], struct best_time fastest_lap[1]) {
    int shared_cars_id = shmget(2, sizeof(car) * length, IPC_CREAT | 0666);
    struct car *shared_memory_cars = shmat(shared_cars_id, NULL, 0);

    if (shared_cars_id < 0){
        perror("shmget not working for shared_cars\n");
        exit(-1);
    }

    int best_time_id = shmget(3, sizeof(best_time) * 4, IPC_CREAT | 0666);
    struct best_time *shared_best_times = shmat(best_time_id, NULL, 0);

    if (best_time_id < 0){
        perror("shmget not working for best_time\n");
        exit(-1);
    }

    for (int i_init=0; i_init < 4; i_init++) {
        shared_best_times[i_init] = default_best;
    }

    int pid_fork;
    int index_car;
    int index_lap;
    struct car sorted_cars[length];

    // --- Lancement de la course sur max_lap tours ---
    for (index_lap = 0; index_lap < max_lap; index_lap++) {
        for (index_car = 0; index_car < length; index_car++) {
            pid_fork = fork();
            if (pid_fork < 0) {
                printf("Fork error.\n");
            } else if (pid_fork == 0) {
                break;
            }
        }
        // --- Processus PARENT ---
        if (pid_fork > 0) {
            while (pid_fork = waitpid(-1, NULL, 0)) {
                if (errno == ECHILD)
                    break;
            }
            for (index_car = 0; index_car < length; index_car++) {
                sorted_cars[index_car] = shared_memory_cars[index_car];
            }
            cars_sort_race(sorted_cars, length);
            system("clear");
            printf("  -------------\n");
            printf("  | Lap %2d/%2d |\n", index_lap+1, max_lap);
            printf("  -------------\n");
            race_time_format(sorted_cars, shared_best_times, length);
        }
        // --- Processus ENFANT ---
        else {
            if (index_lap == 0) {
                shared_memory_cars[index_car] = generate_car(list_cars[index_car]);
            }

            struct car tmp_car = shared_memory_cars[index_car];
            if (!tmp_car.has_crashed) {
                srand(time(NULL) ^ getpid());
                int crash_luck = rand() % 1000;
                if (crash_luck < 1) {
                    tmp_car.tt = 9999999;
                    tmp_car.has_crashed = true;
                } else {
                    run_a_lap(&tmp_car, shared_best_times);
                }
                shared_memory_cars[index_car] = tmp_car;
            }
            exit(0);
        }
        sleep(1);
    }

    // On recopie le tableau final dans qualifierd_cars
    for(int i = 0; i < length; i++) {
        qualifierd_cars[i] = sorted_cars[i];
    }
    fastest_lap[0] = shared_best_times[3];

    // Libération de la mémoire partagée
    shmdt(shared_memory_cars);
    shmctl(shared_cars_id, IPC_RMID, 0);
    shmdt(shared_best_times);
    shmctl(best_time_id, IPC_RMID, 0);
}

void run_a_qualif(int list_cars[], int length, int TOTAL_TIME, struct car qualified_cars[]) {
    int shared_cars_id = shmget(2, sizeof(car) * length, IPC_CREAT | 0666);
    struct car *shared_cars = shmat(shared_cars_id, NULL, 0);

    if (shared_cars_id < 0){
        perror("shmget not working for shared_cars\n");
        exit(-1);
    }

    int best_time_id = shmget(3, sizeof(best_time) * 4, IPC_CREAT | 0666);
    struct best_time *shared_best_times = shmat(best_time_id, NULL, 0);

    if (best_time_id < 0){
        perror("shmget not working for best_time\n");
        exit(-1);
    }

    int sem_reader_id = shmget (4, sizeof(int), 0644 | IPC_CREAT);
    sem_t *sem_reader;
    int *val_sem_reader;

    if (sem_reader_id < 0){
        perror("shmget not working for sem_reader\n");
        exit(-1);
    }

    val_sem_reader = (int *) shmat(sem_reader_id, NULL, 0);
    *val_sem_reader = 0;
    sem_reader = sem_open("p_sem_reader", O_CREAT | O_EXCL, 0644, 1);

    int sem_writer_id = shmget (5, sizeof(int), 0644 | IPC_CREAT);
    sem_t *sem_writer;
    int *val_sem_writer;

    if (sem_writer_id < 0){
        perror("shmget not working for sem_writer\n");
        exit(-1);
    }

    val_sem_writer = (int *) shmat(sem_writer_id, NULL, 0);
    *val_sem_writer = 0;
    sem_writer = sem_open("p_sem_writer", O_CREAT | O_EXCL, 0644, length);

    for (int i_init=0; i_init < 4; i_init++) {
        shared_best_times[i_init] = default_best;
    }

    int child_pid;
    double elapsed_time = 0;
    time_t time_before, time_after;
    time(&time_before);

    // Création des voitures
    for (int i_car=0; i_car < length; i_car++) {
        shared_cars[i_car] = generate_car(list_cars[i_car]);
        child_pid = fork();
        if (child_pid < 0) {
            sem_unlink("p_sem_reader");
            sem_close(sem_reader);
            sem_unlink("p_sem_writer");
            sem_close(sem_writer);
            printf("Fork error.\n");
        } else if (child_pid == 0) {
            // Processus enfant
            break;
        }
    }

    // --- Processus parent ---
    if (child_pid > 0) {
        struct car sorted_cars[length];
        int current_writers;
        
        while(elapsed_time < TOTAL_TIME) {
            sleep(1);
            sem_wait(sem_reader);
            sem_getvalue(sem_writer, &current_writers);

            // On attend que tous les writers aient fini
            while (current_writers != length){
                sem_getvalue(sem_writer, &current_writers);
            }
            for (int idx=0; idx < length; idx++) {
                sorted_cars[idx] = shared_cars[idx];
            }
            cars_sort_qualif(sorted_cars, length);
            system("clear");
            qualif_time_format(sorted_cars, shared_best_times, length);

            sem_post(sem_reader);
            time(&time_after);
            elapsed_time = difftime(time_after, time_before);
        }

        for(int i_copy = 0; i_copy < length; i_copy++) {
            qualified_cars[i_copy] = sorted_cars[i_copy];
        }
    }
    // --- Processus enfant ---
    else {
        int current_readers;

        struct car local_car = shared_cars[getpid() % length];
        srand(time(NULL) ^ getpid());

        while (elapsed_time < TOTAL_TIME){
            sem_getvalue(sem_reader, &current_readers);
            while (current_readers < 1){
                sem_getvalue(sem_writer, &current_readers);
                time(&time_after);
                elapsed_time = difftime(time_after, time_before);
                if (elapsed_time >= TOTAL_TIME){
                    break;
                }
            }

            sem_wait(sem_writer);
            int idx_child = -1;
            // On cherche l'index de la voiture associée au bon pid (simple contournement)
            // car dans la boucle fork, l'enfant reçoit le même getpid() % length potentiellement
            // que d'autres. Dans l'original, l'ordre restait cohérent, on le garde comme tel.
            for (int k=0; k<length; k++){
                if (shared_cars[k].number == local_car.number){
                    idx_child = k;
                    break;
                }
            }
            if (idx_child != -1){
                run_a_lap(&local_car, shared_best_times);
                shared_cars[idx_child] = local_car;
            }
            sem_post(sem_writer);

            sleep(1);
            time(&time_after);
            elapsed_time = difftime(time_after, time_before);
        }
        exit(0);
    }

    // Nettoyage
    shmdt(shared_cars);
    shmctl(shared_cars_id, IPC_RMID, 0);
    shmdt(shared_best_times);
    shmctl(best_time_id, IPC_RMID, 0);

    shmdt(val_sem_reader);
    shmctl(sem_reader_id, IPC_RMID, 0);
    shmdt(val_sem_writer);
    shmctl(sem_writer_id, IPC_RMID, 0);

    sem_unlink("p_sem_reader");
    sem_close(sem_reader);
    sem_unlink("p_sem_writer");
    sem_close(sem_writer);
}
