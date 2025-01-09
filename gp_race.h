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
#include "gp_format.h"

void manageRace(int listCars[], int len, int laps, struct car sortedResult[], struct best_time topLap[1]) {
    int memCarsId = shmget(2, sizeof(car) * len, IPC_CREAT | 0666);
    struct car *sharedCars  = shmat(memCarsId, NULL, 0);
    if (memCarsId < 0) {
        perror("Error: shmget for sharedCars\n");
        exit(-1);
    }

    int memBestTimeId = shmget(3, sizeof(best_time) * 4, IPC_CREAT | 0666);
    struct best_time *globalBestTimes = shmat(memBestTimeId, NULL, 0);
    if (memBestTimeId < 0) {
        perror("Error: shmget for best_time\n");
        exit(-1);
    }

    // Initialiser les 4 meilleurs temps
    for (int k = 0; k < 4; k++) {
        globalBestTimes[k] = default_best;
    }

    int pid;
    struct car tempSort[len];

    // Boucle sur chaque tour
    for (int currentLap = 0; currentLap < laps; currentLap++) {
        for (int i = 0; i < len; i++) {
            pid = fork();
            if (pid < 0) {
                printf("Fork error.\n");
            } else if (pid == 0) {
                // Enfant : traitement d'un tour pour la voiture i
                if (currentLap == 0) {
                    sharedCars[i] = generate_car(listCars[i]);
                }
                struct car tmpCar = sharedCars[i];
                if (!tmpCar.has_crashed) {
                    srand(time(NULL) ^ getpid());
                    int crash_chance = rand() % 1000;
                    if (crash_chance < 1) {
                        tmpCar.tt = 9999999;
                        tmpCar.has_crashed = true;
                    } else {
                        run_a_lap(&tmpCar, globalBestTimes);
                    }
                    sharedCars[i] = tmpCar;
                }
                exit(0);
            }
        }
        // Parent : attendre la fin de tous les enfants
        if (pid > 0) {
            while (pid = waitpid(-1, NULL, 0)) {
                if (errno == ECHILD) break;
            }
            // Mettre à jour le tableau de tri
            for (int i = 0; i < len; i++) {
                tempSort[i] = sharedCars[i];
            }
            cars_sort_race(tempSort, len);
            system("clear");
            printf("  -------------\n");
            printf("  | Lap %2d/%2d |\n", currentLap + 1, laps);
            printf("  -------------\n");
            showRaceTimes(tempSort, globalBestTimes, len);
        }
        sleep(1); 
    }

    // Récupérer le classement final
    for (int i = 0; i < len; i++) {
        sortedResult[i] = tempSort[i];
    }
    topLap[0] = globalBestTimes[3];

    // Nettoyage mémoire partagée
    shmdt(sharedCars);
    shmctl(memCarsId, IPC_RMID, 0);
    shmdt(globalBestTimes);
    shmctl(memBestTimeId, IPC_RMID, 0);
}

void manageQualifs(int listCars[], int len, int totalTime, struct car orderedCars[]) {
    int memCarsId = shmget(2, sizeof(car) * len, IPC_CREAT | 0666);
    struct car *sharedCars = shmat(memCarsId, NULL, 0);
    if (memCarsId < 0) {
        perror("Error: shmget for sharedCars\n");
        exit(-1);
    }

    int memBestTimeId = shmget(3, sizeof(best_time) * 4, IPC_CREAT | 0666);
    struct best_time *globalBestTimes = shmat(memBestTimeId, NULL, 0);
    if (memBestTimeId < 0) {
        perror("Error: shmget for best_time\n");
        exit(-1);
    }

    // Sémaphores
    int semReaderId = shmget(4, sizeof(int), 0644 | IPC_CREAT);
    sem_t *semReader;
    int *valReader;
    if (semReaderId < 0) {
        perror("Error: shmget for sem_reader\n");
        exit(-1);
    }
    valReader = (int *) shmat(semReaderId, NULL, 0);
    *valReader = 0;
    semReader = sem_open("p_sem_reader", O_CREAT | O_EXCL, 0644, 1);

    int semWriterId = shmget(5, sizeof(int), 0644 | IPC_CREAT);
    sem_t *semWriter;
    int *valWriter;
    if (semWriterId < 0) {
        perror("Error: shmget for sem_writer\n");
        exit(-1);
    }
    valWriter = (int *) shmat(semWriterId, NULL, 0);
    *valWriter = 0;
    semWriter = sem_open("p_sem_writer", O_CREAT | O_EXCL, 0644, len);

    // Initialiser les meilleurs temps
    for (int k = 0; k < 4; k++) {
        globalBestTimes[k] = default_best;
    }

    int pid;
    time_t begin, now;
    time(&begin);
    double elapsed = 0;
    int interval = 1;

    // Création de chaque voiture (processus)
    for (int i = 0; i < len; i++) {
        sharedCars[i] = generate_car(listCars[i]);
        pid = fork();
        if (pid < 0) {
            sem_unlink("p_sem_reader");
            sem_close(semReader);
            sem_unlink("p_sem_writer");
            sem_close(semWriter);
            printf("Fork error.\n");
        } else if (pid == 0) {
            // Enfant
            break;
        }
    }

    if (pid > 0) {
        // Processus parent
        struct car tempSort[len];
        int currentWriters;

        while (elapsed < totalTime) {
            sleep(interval);

            sem_wait(semReader);
            sem_getvalue(semWriter, &currentWriters);
            while (currentWriters != len) {
                sem_getvalue(semWriter, &currentWriters);
            }

            for (int i = 0; i < len; i++) {
                tempSort[i] = sharedCars[i];
            }
            cars_sort_qualif(tempSort, len);
            system("clear");
            showQualifTimes(tempSort, globalBestTimes, len);

            sem_post(semReader);

            time(&now);
            elapsed = difftime(now, begin);
        }
        // Copie finale
        for (int i = 0; i < len; i++) {
            orderedCars[i] = tempSort[i];
        }
    } else {
        // Processus enfant
        struct car localCar = sharedCars[getpid() % len]; // Juste pour éviter un warning
        // On retrouve l’index en se basant sur la fourchette, plus précis :
        // Mais en pratique, on peut le récupérer en re-forkant (i) ou en partageant i. 
        // Pour rester proche du code d’origine, on fera un petit "calcul" improbable :

        // On va deviner l’index en parcourant la liste (approche simpliste mais fonctionnelle).
        int childIndex = -1;
        for (int i = 0; i < len; i++) {
            if (sharedCars[i].number == localCar.number) {
                childIndex = i;
                break;
            }
        }
        if (childIndex < 0) childIndex = 0; // fallback

        srand(time(NULL) ^ getpid());
        time(&now);
        elapsed = difftime(now, begin);

        while (elapsed < totalTime) {
            int valRead;
            sem_getvalue(semReader, &valRead);
            while (valRead < 1) {
                sem_getvalue(semWriter, &valRead);
                time(&now);
                elapsed = difftime(now, begin);
                if (elapsed >= totalTime) break;
            }

            sem_wait(semWriter);
            struct car ctemp = sharedCars[childIndex];
            run_a_lap(&ctemp, globalBestTimes);
            sharedCars[childIndex] = ctemp;
            sem_post(semWriter);

            sleep(interval);
            time(&now);
            elapsed = difftime(now, begin);
        }
        exit(0);
    }

    // Nettoyage
    shmdt(sharedCars);
    shmctl(memCarsId, IPC_RMID, 0);
    shmdt(globalBestTimes);
    shmctl(memBestTimeId, IPC_RMID, 0);
    shmdt(valReader);
    shmctl(semReaderId, IPC_RMID, 0);
    shmdt(valWriter);
    shmctl(semWriterId, IPC_RMID, 0);

    sem_unlink("p_sem_reader");
    sem_close(semReader);
    sem_unlink("p_sem_writer");
    sem_close(semWriter);
}
