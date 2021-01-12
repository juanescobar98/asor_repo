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


int main(int argc, char **argv){
    
    sigset_t blk_set, sigActivated_set;
    int error;
    error = sigemptyset(&blk_set);
    if (error == -1) {
        perror("sigemptyset");
        exit(EXIT_FAILURE);
    }
    
    error = sigaddset(&blk_set, SIGINT);
    if (error == -1) {
        perror("sigaddset");
        exit(EXIT_FAILURE);
    }

    error = sigaddset(&blk_set, SIGTSTP);
    if (error == -1) {
        perror("sigaddset");
        exit(EXIT_FAILURE);
    }

    error = sigprocmask(SIG_BLOCK, &blk_set, NULL);
    if (error == -1) {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }

    char * sleepSecs;
    sleepSecs = getenv("SLEEP_SECS");

    if (sleepSecs == NULL) {
        perror("getenv");
        exit(EXIT_FAILURE);
    }

    int segundos;
    segundos = atoi(sleepSecs);
    sleep(segundos);
    
    error = sigpending(&sigActivated_set);
    if (error == -1) {
        perror("sigpending");
        exit(EXIT_FAILURE);
    }

    error = sigismember(&sigActivated_set, SIGINT);
    if (error == -1) {
        perror("sigismember");
        exit(EXIT_FAILURE);
    }
    else if (error == 1)
    {
        printf("Señal SIGINT activada!\n");
    }
    
    error = sigismember(&sigActivated_set, SIGTSTP);
    if (error == -1) {
        perror("sigismember");
        exit(EXIT_FAILURE);
    }
    else if (error == 1)
    {
        printf("Señal SIGTSTP activada!\n");

        error = sigdelset(&blk_set, SIGINT);
        if (error == -1) {
            perror("sigdelset");
            exit(EXIT_FAILURE);
        }

        error = sigprocmask(SIG_UNBLOCK, &blk_set, NULL);
        if (error == -1) {
            perror("sigprocmask");
            exit(EXIT_FAILURE);
        }
        
    }
    
    printf("Final del programa...\n");

    return 0;
}