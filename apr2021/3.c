#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>

#define QUEUE_KEY 10201
#define BUF 100

struct msgbuf {
    long tip;
    char text[100];
};

int main(int argc, char* argv[])
{
    int msgid;
    char msg[BUF];
    FILE* f;
    int pid;
    msgid = msgget(QUEUE_KEY, 0666 | IPC_CREAT);
    if (msgid == -1) {
        printf("Nije napravljen queue\n");
        exit(1);
    }

    pid = fork();
    if(pid > 0)
    {
        struct msgbuf buf;
        buf.tip = 1;
        printf("Roditelj proces -------\n");
        f = fopen(argv[1], "r");
        if (!f) {
            printf("Fajl ne postoji\n");
            exit(1);
        }

        struct msgbuf recvBuf;

        while (fgets(msg, BUF, f) != NULL)
        {
            strcpy(buf.text, msg);

            if (msgsnd(msgid, &buf, strlen(buf.text) + 1, 0) == -1) {
                printf("Nije poslata\n");
            } else {
                printf("Poruka je poslata: %s\n", buf.text);
            }


            if (msgrcv(msgid, &recvBuf, BUF, 0, 0) != -1) {
                printf("Roditelj primio odgovor: %s\n", recvBuf.text);
            } else {
                printf("Roditelj nije primio odgovor\n");
            }

        }
        fclose(f);
        printf("Roditelj proces -------\n");

    }
    else if(pid == 0)
    {
        struct msgbuf buf;
        struct msgbuf sendBuf;
        char recvMsg[BUF];
                printf("Dete proces -------\n");
        int line_num = 1;
        while (1)
        {
            if (msgrcv(msgid, &buf, BUF, 0, 0) != -1)
            {
                printf("Dete primilo poruku: %s\n", buf.text);
                strcpy(recvMsg, buf.text);


                int count_upper = 0;
                printf("Proveravam %s \n", recvMsg);
                for (int i = 0; recvMsg[i] != '\0'; i++) {
                    if(recvMsg[i] >= 'A' && recvMsg[i]<='Z')
                        count_upper++;
                }
                sendBuf.tip = 1;
                sprintf(sendBuf.text, "Linija: %d, Broj velikih slova %d \n", line_num, count_upper);
                if(msgsnd(msgid, &sendBuf, BUF, 0) != -1)
                    printf("Dete vratilo poruku roditelju %s \n", sendBuf.text);

                line_num++;
            }


        }
    }
    else
    {
        printf("Los pid");
    }
    printf("KRAJ\n");
    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}

