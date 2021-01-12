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

*/


volatile int borrar = 1;

void manejo(int signal){
    if (signal == SIGUSR1) {
        printf("Se ha cancelado la eliminacion del archivo\n");
        borrar = 0;
    }
}

int main(int argc, char **argv){
    
    if (argc != 2) {
        fprintf(stderr, "Uso: ./ejercicio13 numSegundos\n");
        exit(EXIT_FAILURE);
    }
    
    int segundos, error;

    struct sigaction accion;
    accion.sa_handler = manejo;
    
    error = sigaction(SIGALRM, &accion, NULL);
    if (error == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    error = sigaction(SIGUSR1, &accion, NULL);
    if (error == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    segundos = atoi(argv[1]);

    alarm(segundos);

    sigset_t mascara;
    error = sigfillset(&mascara);
    if (error == -1) {
        perror("sigfillset");
        exit(EXIT_FAILURE);
    }

    error = sigdelset(&mascara, SIGUSR1);
    if (error == -1) {
        perror("sigdelset");
        exit(EXIT_FAILURE);
    }

    error = sigdelset(&mascara, SIGALRM);
    if (error == -1) {
        perror("sigdelset");
        exit(EXIT_FAILURE);
    }
    // descomentar si se quiere bloquear el resto de señales
    // error = sigprocmask(SIG_BLOCK, &mascara, NULL);
    // if (error == -1) {
    //     perror("sigprocmask");
    //     exit(EXIT_FAILURE);
    // }

    sigsuspend(&mascara);
    

    if (borrar) {
        printf("Alarma!!!!!\n");
        printf("Eliminando...\n");
        remove(argv[0]);
    }
    


    return 0;
}