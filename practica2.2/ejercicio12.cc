#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysmacros.h>


int main(int argc, char **argv){

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int error, fichero;
    fichero = open(argv[1], O_CREAT | O_WRONLY, 0777);

    if (fichero == -1) {
        perror("open");
        return 1;
    }
    error = dup2(fichero, 1);
    if (error == -1) {
        perror("dup2");
        return 1;
    }

    printf("HOLA\n");
    printf("Probando si funciona...\n");
    printf("Hasta luego\n");



    return 0;
}