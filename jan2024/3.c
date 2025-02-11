#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define QUEUE_KEY 10201
#define BUF 100

//Uradio sam ga tako da deca procesi u svakoj iteraciji ispisuju min i max koji su primili od roditelja

struct msgbuf {
    long tip;
    char text[100];
};

int main()
{
    int msgid;
    int pid1, pid2;
    FILE* f;
    char linija[BUF];
    char zaDeljenje[BUF];
    struct msgbuf message;
    int indeks;
    msgid = msgget(QUEUE_KEY, 0666 | IPC_CREAT);
    if(msgid == -1){
        perror("Nije napravljen queue");
        exit(1);
    }

    pid1 = fork();
    if(pid1==0)
    {
        struct msgbuf recMsg;
        char pom[BUF];
        char *tok;
        int indeks;
        int min = 30000;
        int max = -1;
        while(1){
            if(msgrcv(msgid, &recMsg, BUF, 1, 0) != -1){
                    printf("A1 je primio %s\n", recMsg.text);

                    strcpy(pom, recMsg.text);
                    int i = 0;
                    tok = strtok(pom, " ");
                    while(tok!=NULL){
                        if(i == 2)
                            indeks = atoi(tok);
                        tok = strtok(NULL, " ");
                        ++i;
                    }
                    if(indeks > max)
                        max = indeks;

                    if(indeks < min)
                        min = indeks;
            }
            else
                break;
            printf("A1 -----\n");
            printf("Najveci indeks: %d ; Najmanji indeks je %d\n", max, min);

        }

        return 0;
    }

    pid2 = fork();
    if(pid2==0)
    {
        struct msgbuf recMsg;
        char pom[BUF];
        char *tok;
        int indeks;
        int min = 30000;
        int max = -1;
        while(1){
            if(msgrcv(msgid, &recMsg, BUF, 2, 0) != -1){
                printf("A2 je primio %s\n", recMsg.text);

                strcpy(pom, recMsg.text);
                int i = 0;
                tok = strtok(pom, " ");
                while(tok!=NULL){
                    if(i == 2)
                        indeks = atoi(tok);
                    tok = strtok(NULL, " ");
                    ++i;
                }
                if(indeks > max)
                    max = indeks;

                if(indeks < min)
                    min = indeks;
            }
            else
                break;
            printf("A2 -----\n");
            printf("Najveci indeks: %d ; Najmanji indeks je %d\n", max, min);


        }


        return 0;

    }

    f = fopen("prijava.txt", "r");
    if(!f)
        exit(1);

    while(fgets(linija, BUF, f))
    {
        printf("%s \n", linija);
        strcpy(zaDeljenje, linija);
        int i = 0;
        char * tok = strtok(zaDeljenje, " ");
        while(tok!=NULL){
            if(i == 2)
                indeks = atoi(tok);
            tok = strtok(NULL, " ");
            ++i;
        }

        strcpy(message.text, linija);

        if(indeks < 18000)
            message.tip = 1;
        else
            message.tip = 2;

        if(msgsnd(msgid, &message, strlen(message.text)+1, 0) == -1)
                printf("Poruka nije poslata\n");
            else
                printf("Poruka je poslata %s\n");
        printf("------------\n");
    }

    fclose(f);
    wait(0);
    wait(0);
    printf("KRAJ\n");
    msgctl(msgid, IPC_RMID, NULL);
    return 0;

}
