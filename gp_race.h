#ifndef GP_RACE_H
#define GP_RACE_H

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
#include "gp_format.h"   // pour saveSessionResults, showRaceTimes, etc.

void manageRace(int list_cars[], int length, int max_lap,
                struct car finalSorted[],
                struct best_time bestTimesOutput[4]);

void manageQualifs(int list_cars[], int length, int totalTime,
                   struct car qualified_cars[],
                   struct best_time bestTimesOut[4]);

/* --------------------------------------------------------------------------
   Implémentation de manageRace
   -------------------------------------------------------------------------- */
void manageRace(int list_cars[], int length, int max_lap,
                struct car finalSorted[],
                struct best_time bestTimesOutput[4])
{
    /* --- Allocation/attachement SHM pour les voitures --- */
    int memCarsId = shmget(2, sizeof(struct car) * length, IPC_CREAT | 0666);
    if (memCarsId < 0) {
        perror("Error: shmget for sharedCars\n");
        exit(-1);
    }
    struct car *sharedCars = shmat(memCarsId, NULL, 0);

    /* --- Allocation/attachement SHM pour les meilleurs temps --- */
    int memBestId = shmget(3, sizeof(struct best_time) * 4, IPC_CREAT | 0666);
    if (memBestId < 0) {
        perror("Error: shmget for best_time\n");
        exit(-1);
    }
    struct best_time *globalBestTimes = shmat(memBestId, NULL, 0);

    // On réinitialise les 4 meilleurs temps
    for (int k = 0; k < 4; k++) {
        globalBestTimes[k] = default_best; 
    }

    /* --- Simulation de la course sur 'max_lap' tours --- */
    pid_t pid;
    struct car sortedTemp[length];

    for (int currentLap = 0; currentLap < max_lap; currentLap++) {
        // Pour chaque voiture, on fork
        for (int i = 0; i < length; i++) {
            pid = fork();
            if (pid < 0) {
                perror("Fork error");
            }
            else if (pid == 0) {
                // Enfant : gère 1 tour pour la voiture i
                if (currentLap == 0) {
                    // Première fois : on initialise la voiture
                    sharedCars[i] = generate_car(list_cars[i]);
                }
                // Récupère la structure
                struct car tmpCar = sharedCars[i];

                // Vérifie si déjà OUT
                if (!tmpCar.has_crashed) {
                    srand(time(NULL) ^ getpid());
                    // On simule un éventuel abandon
                    int crash_chance = rand() % 1000;
                    if (crash_chance < 1) {  // ~0.1% de chance
                        tmpCar.tt = 9999999;
                        tmpCar.has_crashed = true;
                    } else {
                        // Fait un tour et met à jour le globalBestTimes
                        run_a_lap(&tmpCar, globalBestTimes);
                    }
                    sharedCars[i] = tmpCar;
                }
                exit(0); // Fini le processus enfant
            }
        }
        // Le parent attend la fin de tous les enfants
        if (pid > 0) {
            while (waitpid(-1, NULL, 0)) {
                if (errno == ECHILD) break;
            }
            // Mise à jour du tableau local sortedTemp
            for (int i = 0; i < length; i++) {
                sortedTemp[i] = sharedCars[i];
            }
            // On trie par TT pour avoir le classement
            cars_sort_race(sortedTemp, length);

            // Affichage "live" dans le terminal
            system("clear");
            printf("  -------------\n");
            printf("  | Lap %2d/%2d |\n", currentLap + 1, max_lap);
            printf("  -------------\n");
            showRaceTimes(sortedTemp, globalBestTimes, length);
        }
        sleep(1); // petit délai entre les tours
    }

    // À la fin de la course, on recopie l'ordre final dans finalSorted
    for (int i = 0; i < length; i++) {
        finalSorted[i] = sortedTemp[i];
    }
    // On recopie aussi les 4 meilleurs temps (S1, S2, S3, lap)
    for (int i = 0; i < 4; i++) {
        bestTimesOutput[i] = globalBestTimes[i];
    }

    // Libération des segments de mémoire
    shmdt(sharedCars);
    shmctl(memCarsId, IPC_RMID, 0);
    shmdt(globalBestTimes);
    shmctl(memBestId, IPC_RMID, 0);
}

/* --------------------------------------------------------------------------
   Implémentation de manageQualifs
   -------------------------------------------------------------------------- */
void manageQualifs(int list_cars[], int length, int totalTime,
                   struct car qualified_cars[],
                   struct best_time bestTimesOut[4])
{
    /* --- Allocation/attachement SHM pour les voitures --- */
    int memCarsId = shmget(2, sizeof(struct car) * length, IPC_CREAT | 0666);
    if (memCarsId < 0){
        perror("shmget not working for shared_cars\n");
        exit(-1);
    }
    struct car *sharedCars = shmat(memCarsId, NULL, 0);

    /* --- Allocation/attachement SHM pour les meilleurs temps --- */
    int memBestId = shmget(3, sizeof(struct best_time) * 4, IPC_CREAT | 0666);
    if (memBestId < 0){
        perror("shmget not working for best_time\n");
        exit(-1);
    }
    struct best_time *globalBestTimes = shmat(memBestId, NULL, 0);

    // Réinitialise
    for (int k = 0; k < 4; k++) {
        globalBestTimes[k] = default_best;
    }

    // Sémaphores pour la synchro parent/child
    int semReaderId = shmget(4, sizeof(int), 0644 | IPC_CREAT);
    if (semReaderId < 0){
        perror("shmget not working for sem_reader\n");
        exit(-1);
    }
    int *valReader = (int*) shmat(semReaderId, NULL, 0);
    *valReader = 0; 
    sem_t *semReader = sem_open("p_sem_reader", O_CREAT | O_EXCL, 0644, 1);

    int semWriterId = shmget(5, sizeof(int), 0644 | IPC_CREAT);
    if (semWriterId < 0){
        perror("shmget not working for sem_writer\n");
        exit(-1);
    }
    int *valWriter = (int*) shmat(semWriterId, NULL, 0);
    *valWriter = 0;
    sem_t *semWriter = sem_open("p_sem_writer", O_CREAT | O_EXCL, 0644, length);

    // On crée + init les voitures
    for (int i = 0; i < length; i++) {
        sharedCars[i] = generate_car(list_cars[i]);
    }

    pid_t pid;
    // On lance un fork par voiture
    for (int i = 0; i < length; i++) {
        pid = fork();
        if (pid < 0) {
            perror("Fork error in manageQualifs\n");
            // Libération sem pour éviter blocage
            sem_unlink("p_sem_reader");
            sem_close(semReader);
            sem_unlink("p_sem_writer");
            sem_close(semWriter);
        }
        else if (pid == 0) {
            // Processus enfant
            // On boucle tant que la session n'est pas terminée
            time_t start, now;
            time(&start);
            double elapsed = 0;
            srand(time(NULL) ^ getpid());

            while (elapsed < totalTime) {
                // On attend que le parent ne soit pas en lecture
                int valRead;
                sem_getvalue(semReader, &valRead);
                // Tant que semReader < 1, on attend
                while (valRead < 1) {
                    sem_getvalue(semReader, &valRead);
                    time(&now);
                    elapsed = difftime(now, start);
                    if (elapsed >= totalTime) break;
                }

                // On lock en écriture
                sem_wait(semWriter);
                // On effectue un tour
                struct car tmp = sharedCars[i];
                run_a_lap(&tmp, globalBestTimes);
                sharedCars[i] = tmp;
                // On libère
                sem_post(semWriter);

                sleep(1);
                time(&now);
                elapsed = difftime(now, start);
            }
            exit(0);
        }
    }

    // Processus parent
    time_t begin, curr;
    time(&begin);
    double elapsed = 0;
    struct car sortedTemp[length];
    int currentWriters;

    while (elapsed < totalTime) {
        sleep(1);

        // On lock en lecture
        sem_wait(semReader);
        // On s'assure qu'il n'y a plus d'écriture en cours
        sem_getvalue(semWriter, &currentWriters);
        while (currentWriters != length) {
            sem_getvalue(semWriter, &currentWriters);
        }

        // Copie locale
        for (int i = 0; i < length; i++) {
            sortedTemp[i] = sharedCars[i];
        }
        // Trie par BT
        cars_sort_qualif(sortedTemp, length);
        // Affichage
        system("clear");
        showQualifTimes(sortedTemp, globalBestTimes, length);

        sem_post(semReader);

        time(&curr);
        elapsed = difftime(curr, begin);
    }

    // On attend la fin des enfants
    while (waitpid(-1, NULL, 0)) {
        if (errno == ECHILD) break;
    }

    // On recopie le classement final (trié) dans qualified_cars
    // (pour que la suite sache qui est éliminé, etc.)
    for (int i = 0; i < length; i++) {
        qualified_cars[i] = sortedTemp[i];
    }

    // On renvoie aussi les 4 meilleurs temps
    for (int i = 0; i < 4; i++) {
        bestTimesOut[i] = globalBestTimes[i];
    }

    // Nettoyage de la SHM
    shmdt(sharedCars);
    shmctl(memCarsId, IPC_RMID, 0);
    shmdt(globalBestTimes);
    shmctl(memBestId, IPC_RMID, 0);

    // Nettoyage des semaphores
    shmdt(valReader);
    shmctl(semReaderId, IPC_RMID, 0);
    shmdt(valWriter);
    shmctl(semWriterId, IPC_RMID, 0);
    sem_unlink("p_sem_reader");
    sem_close(semReader);
    sem_unlink("p_sem_writer");
    sem_close(semWriter);
}

#endif // GP_RACE_H
