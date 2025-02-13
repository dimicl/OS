#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#define BUF 10

void* odbrojavanje(void* broj)
{
    char* number = (char*)broj;
    int num;
    num = atoi(number);
    printf("Pocinje odbrojavanje...\n");
    for(int i = num; i >= 0; i--)
    {
        printf("%d\n", i);
        sleep(3);
    }
    return 0;
}

int main()
{
    char buf[BUF];
    pthread_t nit;
    printf("Unesite broj\n");
    gets(buf);

    while(strcmp(buf, "KRAJ\n") != 0)
    {
        pthread_create(&nit, NULL, odbrojavanje, (void*)&buf);
        pthread_join(nit, NULL);

        printf("Unesite broj\n");
        gets(buf);
        if(strcmp(buf, "KRAJ") == 0)
            break;

    }

    return 0;
}
