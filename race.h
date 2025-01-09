#include <stdlib.h>
#include <stdio.h>
#include <time.h> 
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/types.h>      /* key_t, sem_t, pid_t */
#include <sys/shm.h>        /* shmat(), IPC_RMID   */
#include <errno.h>          /* errno, ECHILD       */
#include <semaphore.h>      /* sem_open(), sem_post(), sem_wait().. */
#include <fcntl.h>          /* O_CREAT, O_EXCL     */
#include <sys/wait.h>       /* waitpid()           */
#include "format.h"

// ----------------------------------------------------------------------------
// ---------------------  COURSE (RACE)  --------------------------------------
// ----------------------------------------------------------------------------

void run_a_race(int list_cars[], int length, int max_lap,
                struct car qualifierd_cars[], struct best_time fastest_lap[1]) {

    // --- Création mémoire partagée pour les voitures
    int shared_cars_id = shmget(2, sizeof(car) * length, IPC_CREAT | 0666);
    if (shared_cars_id < 0){
        perror("shmget not working for shared_cars\n");
        exit(-1);
    }
    struct car *shared_memory_cars = shmat(shared_cars_id, NULL, 0);

    // --- Création mémoire partagée pour best_time
    int best_time_id = shmget(3, sizeof(best_time) * 4, IPC_CREAT | 0666);
    if (best_time_id < 0){
        perror("shmget not working for best_time\n");
        exit(-1);
    }
    struct best_time *shared_best_times = shmat(best_time_id, NULL, 0);

    // Initialisation des meilleurs temps
    for (int i_init = 0; i_init < 4; i_init++) {
        shared_best_times[i_init] = default_best; 
    }

    // Variables de fork
    int pid_fork;
    struct car sorted_cars[length];

    // --- Lancement de la course sur max_lap tours ---
    for (int current_lap = 0; current_lap < max_lap; current_lap++) {
        
        // On fork autant de fois qu'il y a de voitures
        // pour exécuter run_a_lap() en parallèle (enfant).
        for (int current_car = 0; current_car < length; current_car++) {
            pid_fork = fork();
            if (pid_fork < 0) {
                perror("Fork error.\n");
                exit(-1);
            }
            // Si on est dans le processus enfant, on sort immédiatement du for
            // pour exécuter le code enfant
            if (pid_fork == 0) {
                // Enfant
                // 1) On génère la voiture au 1er tour
                if (current_lap == 0) {
                    shared_memory_cars[current_car] = generate_car(list_cars[current_car]);
                }
                // 2) On récupère la bonne voiture
                struct car tmp_car = shared_memory_cars[current_car];

                // 3) On vérifie si crash ou non
                if (!tmp_car.has_crashed) {
                    srand(time(NULL) ^ getpid());
                    int crash_luck = rand() % 1000;
                    if (crash_luck < 1) {
                        // Crash
                        tmp_car.tt = 9999999;
                        tmp_car.has_crashed = true;
                    } else {
                        // On met à jour s1, s2, s3, bt, tt
                        run_a_lap(&tmp_car, shared_best_times);
                    }
                    shared_memory_cars[current_car] = tmp_car;
                }
                // Fin enfant
                exit(0);
            }
            // Sinon (pid_fork > 0), on est dans le parent -> on continue la boucle
        }

        // --- Processus PARENT : attend la fin de tous les enfants
        if (pid_fork > 0) {
            while (waitpid(-1, NULL, 0)) {
                if (errno == ECHILD) {
                    break;
                }
            }
            // On récupère les voitures mises à jour depuis la mémoire partagée
            for (int c = 0; c < length; c++) {
                sorted_cars[c] = shared_memory_cars[c];
            }
            // Tri selon le temps de course (tt)
            cars_sort_race(sorted_cars, length);

            system("clear");
            printf("  -------------\n");
            printf("  | Lap %2d/%2d |\n", current_lap + 1, max_lap);
            printf("  -------------\n");
            race_time_format(sorted_cars, shared_best_times, length);
        }

        // On met un sleep pour illustrer l’avancement des tours
        sleep(1);
    }

    // Copie des résultats finaux (voitures triées) dans qualifierd_cars
    for(int i = 0; i < length; i++) {
        qualifierd_cars[i] = sorted_cars[i];
    }
    // On récupère le meilleur tour global
    fastest_lap[0] = shared_best_times[3];

    // Libération de la mémoire partagée
    shmdt(shared_memory_cars);
    shmctl(shared_cars_id, IPC_RMID, 0);

    shmdt(shared_best_times);
    shmctl(best_time_id, IPC_RMID, 0);
}

// ----------------------------------------------------------------------------
// ---------------------  QUALIFICATION (QUALIF)  -----------------------------
// ----------------------------------------------------------------------------

void run_a_qualif(int list_cars[], int length, int TOTAL_TIME,
                  struct car qualified_cars[]) {

    // --- Création mémoire partagée pour les voitures
    int shared_cars_id = shmget(2, sizeof(car) * length, IPC_CREAT | 0666);
    if (shared_cars_id < 0){
        perror("shmget not working for shared_cars\n");
        exit(-1);
    }
    struct car *shared_cars = shmat(shared_cars_id, NULL, 0);

    // --- Création mémoire partagée pour best_time
    int best_time_id = shmget(3, sizeof(best_time) * 4, IPC_CREAT | 0666);
    if (best_time_id < 0){
        perror("shmget not working for best_time\n");
        exit(-1);
    }
    struct best_time *shared_best_times = shmat(best_time_id, NULL, 0);

    // --- Création mémoire partagée pour lecture/écriture (sémaphores)
    int sem_reader_id = shmget (4, sizeof(int), 0644 | IPC_CREAT);
    if (sem_reader_id < 0){
        perror("shmget not working for sem_reader\n");
        exit(-1);
    }
    int *val_sem_reader = (int *) shmat (sem_reader_id, NULL, 0);
    *val_sem_reader = 0;  // initialisation

    sem_t *sem_reader = sem_open("p_sem_reader", O_CREAT | O_EXCL, 0644, 1);

    int sem_writer_id = shmget (5, sizeof(int), 0644 | IPC_CREAT);
    if (sem_writer_id < 0){
        perror("shmget not working for sem_writer\n");
        exit(-1);
    }
    int *val_sem_writer = (int *) shmat (sem_writer_id, NULL, 0);
    *val_sem_writer = 0; // initialisation

    // On autorise "length" accès en écriture au départ
    sem_t *sem_writer = sem_open("p_sem_writer", O_CREAT | O_EXCL, 0644, length);

    // Initialisation des meilleurs temps
    for (int i_init=0; i_init < 4; i_init++) {
        shared_best_times[i_init] = default_best;
    }

    // On calcule le temps avec un "time_t" avant/après
    time_t time_before, time_after;
    time(&time_before);
    double elapsed_time = 0;

    // Fork pour chaque voiture
    int child_pid = 0; 
    int i_car;
    for (i_car = 0; i_car < length; i_car++) {
        // On crée la voiture dans la mémoire partagée
        shared_cars[i_car] = generate_car(list_cars[i_car]);
        
        child_pid = fork();
        if (child_pid < 0) {
            // Nettoyage s’il y a erreur
            sem_unlink("p_sem_reader");
            sem_close(sem_reader);
            sem_unlink("p_sem_writer");
            sem_close(sem_writer);
            perror("Fork error.\n");
            exit(-1);
        }
        if (child_pid == 0) {
            // Enfant : break -> on traite la voiture i_car
            break;
        }
    }

    // ------------------- Processus PARENT -------------------
    if (child_pid > 0) {

        struct car sorted_cars[length];
        int current_writers = 0;

        // Boucle tant que le temps n’est pas écoulé
        while(elapsed_time < TOTAL_TIME) {
            sleep(1);

            // Verrou "lecture"
            sem_wait(sem_reader);

            // On attend que tous les writers aient fini
            sem_getvalue(sem_writer, &current_writers);
            while (current_writers != length){
                sem_getvalue(sem_writer, &current_writers);
            }

            // On récupère toutes les voitures mises à jour
            for (int idx=0; idx < length; idx++) {
                sorted_cars[idx] = shared_cars[idx];
            }

            // On trie, on affiche
            cars_sort_qualif(sorted_cars, length);
            system("clear");
            qualif_time_format(sorted_cars, shared_best_times, length);

            // Déverrouillage "lecture"
            sem_post(sem_reader);

            time(&time_after);
            elapsed_time = difftime(time_after, time_before);
        }

        // Une fois le temps écoulé, on copie la dernière version triée
        // dans le tableau qualified_cars
        cars_sort_qualif(sorted_cars, length);
        for(int i_copy=0; i_copy < length; i_copy++) {
            qualified_cars[i_copy] = sorted_cars[i_copy];
        }

        // On attend la fin de tous les enfants pour nettoyer
        while (waitpid(-1, NULL, 0)) {
            if (errno == ECHILD) {
                break;
            }
        }
    }

    // ------------------- Processus ENFANT -------------------
    else if (child_pid == 0 && i_car < length) {

        srand(time(NULL) ^ getpid());
        while (elapsed_time < TOTAL_TIME) {

            // Tant que le parent lit, on attend
            int current_readers = 0;
            sem_getvalue(sem_reader, &current_readers);
            while (current_readers < 1) {
                // On vérifie si le temps est déjà écoulé
                time(&time_after);
                elapsed_time = difftime(time_after, time_before);
                if (elapsed_time >= TOTAL_TIME) {
                    // Il n’y a plus de temps de qualification restant
                    break;
                }
                sem_getvalue(sem_writer, &current_readers);
            }

            // Ecriture : on attend le jeton writer
            sem_wait(sem_writer);

            // On récupère la bonne voiture dans la mémoire
            struct car local_car = shared_cars[i_car];
            
            // On effectue le tour, on met à jour local_car
            run_a_lap(&local_car, shared_best_times);

            // On réécrit dans la mémoire partagée
            shared_cars[i_car] = local_car;

            // On libère le jeton writer
            sem_post(sem_writer);

            // Petit délai
            sleep(1);

            // Mise à jour du temps
            time(&time_after);
            elapsed_time = difftime(time_after, time_before);
        }
        // Fin enfant
        exit(0);
    }

    // ------------------- Nettoyage mémoire partagée -------------------
    shmdt(shared_cars);
    shmctl(shared_cars_id, IPC_RMID, 0);

    shmdt(shared_best_times);
    shmctl(best_time_id, IPC_RMID, 0);

    shmdt(val_sem_reader);
    shmctl(sem_reader_id, IPC_RMID, 0);

    shmdt(val_sem_writer);
    shmctl(sem_writer_id, IPC_RMID, 0);

    // Destruction des sémaphores
    sem_unlink("p_sem_reader");
    sem_close(sem_reader);
    sem_unlink("p_sem_writer");
    sem_close(sem_writer);
}
