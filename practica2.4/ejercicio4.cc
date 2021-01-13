#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

/*
Se trata como un fichero, las dos terminales deben estar en el mismo directorio
https://poesiabinaria.net/2009/07/tuberias-con-nombre-para-comunicacion-entre-procesos/

mkfifo prueba

*/

int main(int argc, char **argv){

    if (argc != 2) {
        fprintf(stderr, "Uso: ./ejercicio4 mensaje\n");
        exit(EXIT_FAILURE);
    }

    int error, fichero;

    fichero = open("prueba", O_WRONLY);

    if (fichero == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    int longitud;
    longitud = strlen(argv[1]);

    error = write(fichero, argv[1], longitud);
    if (error == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }
    
    error = close(fichero);
    if (error == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }

    return 0;
}