#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char **argv){

    if (argc != 5) {
        fprintf(stderr, "Uso: ./ejercicio1 comando1 argumento1 comando2 argumento2\n");
        exit(EXIT_FAILURE);
    }

    int tuberia[2];
    int error;
    
    error = pipe(tuberia); //tuberia[1] --> tuberia[0]
    if (error == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    
    pid_t pid;

    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) //Hijo
    {
        error = dup2(tuberia[0], STDIN_FILENO); //entrada estandar -> tuberia[0]
        if (error == -1) {
            perror("dup2 hijo");
            exit(EXIT_FAILURE);
        }

        error = close(tuberia[0]);
        if (error == -1) {
            perror("close");
            exit(EXIT_FAILURE);
        }

        error = close(tuberia[1]);
        if (error == -1) {
            perror("close");
            exit(EXIT_FAILURE);
        }

        execlp(argv[3], argv[3], argv[4], NULL); // (wc, wc, -c, NULL)

    }
    else //Padre
    {
        error = dup2(tuberia[1], STDOUT_FILENO); //salida estandar -> tuberia[1]
        if (error == -1) {
            perror("dup2 padre");
            exit(EXIT_FAILURE);
        }

        error = close(tuberia[0]);
        if (error == -1) {
            perror("close");
            exit(EXIT_FAILURE);
        }

        error = close(tuberia[1]);
        if (error == -1) {
            perror("close");
            exit(EXIT_FAILURE);
        }

        execlp(argv[1], argv[1], argv[2], NULL); // (echo, echo, 12345, NULL)
    }
    
    return 0;
}