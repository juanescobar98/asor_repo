#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
/*
En la consola poner valor a la variable de entorno
SLEEP_SECS=10
export SLEEP_SECS
*/


volatile int numSigints = 0;
volatile int numSigtstp = 0;

void manejo(int signal){
    if (signal == SIGINT) {
        numSigints++;
    }
    else if (signal == SIGTSTP)
    {
        numSigtstp++;
    }
}

int main(int argc, char **argv){
    
    sigset_t blk_set, sigActivated_set;
    int error;
    struct sigaction accion;
    accion.sa_handler = manejo;
    
    error = sigaction(SIGINT, &accion, NULL);
    if (error == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    error = sigaction(SIGTSTP, &accion, NULL);
    if (error == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    

    error = sigprocmask(SIG_BLOCK, &blk_set, NULL);
    if (error == -1) {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }

    sigset_t mascara;
    error = sigemptyset(&mascara);
    if (error == -1) {
        perror("sigemptyset");
        exit(EXIT_FAILURE);
    }
    
    while(numSigints + numSigtstp < 10){
        sigsuspend(&mascara);
    }
    

    printf("Numero de SIGINT: %d\n", numSigints);
    printf("Numero de SIGTSTP: %d\n", numSigtstp);

    return 0;
}