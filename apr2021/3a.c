#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <string.h>

#define QUEUE_KEY 10201
#define BUF 128

struct msgbuf {
    long tip;
    char text[100];
};

int main() {
    int msgid;
    char msg[BUF];

    msgid = msgget(QUEUE_KEY, 0666 | IPC_CREAT);
    if (msgid == -1) {
        printf("Queue nije napravljen\n");
        exit(1);
    }

    struct msgbuf buf;
    struct msgbuf sendBuf;
    while (1) {
        if (msgrcv(msgid, &buf, BUF, 0, 0) != -1) {
            printf("Dete primilo poruku: %s\n", buf.text);
            /*
            int line_num;
            int count_upper = 0;
            char text[BUF];
            sscanf(buf.text, "%d %[^\n]", &line_num, text);

            for (int i = 0; text[i] != '\0'; i++) {
                    count_upper++;
                }
            }
            */
        } else {
            printf("Nije primljena poruka\n");
        }

        sendBuf.tip = 1;
            strcpy(sendBuf.text, "Linija 5, Velikih slova: 2");

            if (msgsnd(msgid, &sendBuf, strlen(sendBuf.text) + 1, 0) == -1) {
                printf("Nije poslato\n");
            }
            else
            {
                printf("Poslao sam %s", sendBuf.text);
            }
    }

    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}
