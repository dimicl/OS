#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <fcntl.h>
#define BUF 1024

int main(int argc, char* argv[])
{
    if(argc != 4)
        exit(1);

    int pd[2];
    int pid;
    int bajtovi = atoi(argv[2]);
    if(pipe(pd) == -1)
        exit(1);

    pid = fork();
    if(pid == -1)
        exit(1);
    if(pid > 0)
    {
        close(pd[0]);
        int source = open(argv[1], O_RDONLY); //koristim deskriptor da mogu da koristim read i write zbog bajtova
        if(source == -1)
            exit(1);

        char buf[BUF];
        int procitano;
        while(1)
        {
            procitano = read(source, buf, bajtovi); //citamo iz izv
            if(procitano == -1)
                exit(1);
            if(procitano == 0)
                break;

            write(pd[1], buf, procitano); //saljemo preko pipe
        }

        close(source);
        close(pd[1]);

        wait(NULL);
    }
    else
    {
        close(pd[1]);
        int destination = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0666); //isto deskriptor
        if(destination == -1)
            exit(1);
        char buf[BUF];
        int procitano;

        while((procitano = read(pd[0], buf, BUF)) > 0){
            sleep(5);
            write(destination, buf, procitano);
        } //kad god upalim procita ceo fajl ne mogu da vidim da li cita po N bajtova , pokusao sam da simuliram s sleep ali nece
        if(procitano == -1)
            exit(1);
        close(destination);
        close(pd[0]);
    }
    return 0;

}
