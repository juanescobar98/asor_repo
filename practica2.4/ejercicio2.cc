#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

/*
Se pone /0 al final porque si no no recibe bien el fin de mensaje cuando hay mensajes largos
y despues mas cortos ya que no contiene /0 al final sino el string anterior
*/

int main(int argc, char **argv){

    if (argc > 1) {
        fprintf(stderr, "Uso: ./ejercicio2\n");
        exit(EXIT_FAILURE);
    }

    int p_h[2], h_p[2];
    int error;
    
    error = pipe(p_h); //p_h[1] --> p_h[0]
    if (error == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    error = pipe(h_p); //h_p[1] --> h_p[0]
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

        error = close(p_h[1]);
        if (error == -1) {
            perror("close");
            exit(EXIT_FAILURE);
        }

        error = close(h_p[0]);
        if (error == -1) {
            perror("close");
            exit(EXIT_FAILURE);
        }

        char mensajeRecibido[CHAR_MAX], mensajeEnviar[1];
        int nBytes;
        mensajeEnviar[0] = 'l';
        for(int i = 0; i < 10; i++)
        {
            nBytes = read(p_h[0], mensajeRecibido, CHAR_MAX);
            if (nBytes == -1) {
                perror("read");
                exit(EXIT_FAILURE);
            }

            mensajeRecibido[nBytes] = '\0';
            printf("Mensaje en el hijo: %s", mensajeRecibido);
            sleep(1);

            if (i == 9) {
                mensajeEnviar[0] = 'q';
            }

            nBytes = write(h_p[1], mensajeEnviar, 1);
            if (nBytes == -1) {
                perror("write");
                exit(EXIT_FAILURE);
            }
            
        }

        error = close(p_h[0]);
        if (error == -1) {
            perror("close");
            exit(EXIT_FAILURE);
        }

        error = close(h_p[1]);
        if (error == -1) {
            perror("close");
            exit(EXIT_FAILURE);
        }
        
    }
    else //Padre
    {

        error = close(p_h[0]);
        if (error == -1) {
            perror("close");
            exit(EXIT_FAILURE);
        }

        error = close(h_p[1]);
        if (error == -1) {
            perror("close");
            exit(EXIT_FAILURE);
        }

        char mensaje[CHAR_MAX + 1], numBytes, mensajeHijo[1];
        mensajeHijo[0] = 'l';

        while(mensajeHijo[0] != 'q'){
            numBytes = read(STDIN_FILENO, mensaje, CHAR_MAX);
            if (numBytes == -1) {
                perror("read");
                exit(EXIT_FAILURE);
            }

            mensaje[numBytes] = '\0';

            numBytes = write(p_h[1], mensaje, numBytes + 1);
            if (numBytes == -1) {
                perror("write");
                exit(EXIT_FAILURE);
            }


            numBytes = read(h_p[0], mensajeHijo, 1);
            if (numBytes == -1) {
                perror("read");
                exit(EXIT_FAILURE);
            }
        }
        

        error = close(p_h[1]);
        if (error == -1) {
            perror("close");
            exit(EXIT_FAILURE);
        }

        error = close(h_p[0]);
        if (error == -1) {
            perror("close");
            exit(EXIT_FAILURE);
        }

    }
    
    return 0;
}