#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#define BUF_SIZE 100

int sum = 0;
void* saberi(void* arg)
{
    int locSum = 0; //lokalna suma da nam bude lakse u petlju
    printf("Nit startovana..\n");

    char* nums = (char*)arg; //cast u char*
    char* tok = strtok(nums, " "); //ja sam radio ovako preko strtok, pokazivac na niz
    while(tok != NULL){ //dok pokazivac nije dosao do kraja
        locSum += atoi(tok); //pretvaramo u int i dodajemo na lokalnu sumu
        tok = strtok(NULL, " "); //od poslednjeg pokazivaca odbacujemo ponovo blanko
    }
    sum = locSum; //dodajemo u globalnu sumu
    sleep(3);
    return NULL;
}

int main()
{
    pthread_t nit;
    FILE* f;
    char buf[BUF_SIZE];
    f = fopen("brojevi.txt", "r");
    while(fgets(buf, BUF_SIZE, f)) //citamo liniju po liniju iz fajla
    {
        sum = 0; //svaki put kad prodje resetujemo global prom sum
        printf("Vasi brojevi su : %s", buf);
        pthread_create(&nit, NULL, saberi, &buf); //saljemo buf u nit

        pthread_join(nit, NULL); //cekamo nit da zavrsi
        printf("Suma ovih brojeva: %d\n", sum); //ispisujemo konacnu sumu
    }
    printf("Program zavrsen. \n");
    return 0;
}
