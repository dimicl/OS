#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#define BUF_SIZE 100
int main()
{
    int pid;
    int pd[2];
    int pd1[2];

    if(pipe(pd) < 0){
        perror("Greska pipe\n");
        exit(1);
    }
     if(pipe(pd1) < 0){
        perror("Greska pipe\n");
        exit(1);
    }
    pid = fork();
    if(pid == 0){
        close(pd[1]);
        close(pd1[0]);
        char buf[BUF_SIZE];
        int loc = 0;
        while(1)
        {
            if(read(pd[0], buf, BUF_SIZE) > 0)
            {
                printf("Dete proces primilo %s\n", buf);
                char sendBuf[BUF_SIZE];
                loc = 0;
                strcpy(sendBuf, buf);
                for(int i = 0; sendBuf[i] != '\0'; i++)
                {
                    if(sendBuf[i] >= '0' && sendBuf[i] <= '9')
                        loc += sendBuf[i] - '0';
                }
                sprintf(sendBuf, "Suma cifara je %d \n", loc);
                write(pd1[1], sendBuf, BUF_SIZE);
            }
            else
            {
                printf("Dete nije primilo\n");
            }

        }



        close(pd[0]);
        close(pd1[1]);
        return NULL;

    }
    else
    {
        close(pd[0]);
        close(pd1[1]);
        char recenica[1024];
        char buf[BUF_SIZE];
        char recvBuf[BUF_SIZE];
        gets(recenica);
        char* tok = strtok(recenica, " ");
        while(tok != NULL){
            strcpy(buf, tok);
            printf("Roditelj proces salje %s\n", buf);
            write(pd[1], buf, BUF_SIZE);

            read(pd1[0],recvBuf, BUF_SIZE);
            printf("Roditelj proces primio %s\n", recvBuf);

            tok = strtok(NULL, " ");

        }
        printf("Roditelj izlazi.\n");
        wait(NULL);
        close(pd[1]);
        close(pd1[0]);
        return 0;

    }
}
