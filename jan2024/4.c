#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#define MUTEX_KEY 10101
#define SHARED_MEM_KEY 10102
#define N 10
union semun {
    int val;
};

int main() {
    int mutexid, shmemid;
    union semun semopts;
    struct sembuf sem_lock = { 0, -1, 0 };
    struct sembuf sem_unlock = { 0, 1, 0 };
    int *shmptr;
    FILE* f;
    int i = 0;
    int row = 0; //ovo cu da koristim za broj redova u fajlu

    mutexid = semget(MUTEX_KEY, 1, 0666 | IPC_CREAT);
    if (mutexid == -1) {
        perror("Semafor nije kreiran");
        exit(1);
    }

    semopts.val = 1; //psotavljamo mutex semafor na 1
    if (semctl(mutexid, 0, SETVAL, semopts) == -1) {
        perror("Semafor nije postavljen");
        exit(1);
    }

    shmemid = shmget(SHARED_MEM_KEY, 1000 * sizeof(int), IPC_CREAT | 0666);
    if (shmemid == -1)
        exit(1);


    shmptr = (int *)shmat(shmemid, NULL, 0); //mapiramo deljenu memoriju ovo int* je jer radimo s int brojevima

    f = fopen("numbers.txt", "r");
    if (!f) {
        perror("Fajl nije pronađen");
        exit(1);
    }

    while (fscanf(f, "%d %d %d %d %d %d %d %d %d %d\n",
           &shmptr[row * N + 0], &shmptr[row * N + 1], &shmptr[row * N + 2],
           &shmptr[row * N + 3], &shmptr[row * N + 4], &shmptr[row * N + 5],
           &shmptr[row * N + 6], &shmptr[row * N + 7], &shmptr[row * N + 8],
           &shmptr[row * N + 9]) == 10) {
        row++;
    } //ovamo iz fajla upisujemo u deljenu memoriju, sve dok ima niza u fajlu povecavamo red, svaki broj iz fajla ide na mesto u deljenu memoriju jer nju gledamo samo kao skup lokacija
    fclose(f);

    if (fork() == 0) {
        int sum = 0;
        int trenutni = 0;

        semop(mutexid, &sem_lock, 1); //blokiramo mutex da bi sinhronizovali procese

        for (trenutni = 0; trenutni< row; trenutni++) {
            sum = 0;
            for (int i = 0; i < N; i++) {
                sum += shmptr[trenutni * N + i]; //sumiramo sve brojeve iz niza
            }
            printf("Suma niza %d: %d\n", trenutni + 1, sum);
        }

        semop(mutexid, &sem_unlock, 1); //odblokiramo jer smo zavrsili
        shmdt(shmptr); //oslobadjamo deljenu memoriju
        exit(0);
    }

    wait(NULL);

    semctl(mutexid, 0, IPC_RMID, 0);
    shmctl(shmemid, IPC_RMID, NULL);

    return 0;
}
