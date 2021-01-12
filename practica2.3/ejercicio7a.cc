#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

/*
Uso correcto poniendo el nombre del comando y los argumentos entre comillas
*/


int main(int argc, char **argv){
    
    int exito;

    if (argc != 2) {
        printf("Uso: ./ejercicio7a \"nombrePrograma arg1 arg2 ...\" \n");
        exit(1);
    }
    
    exito = system(argv[1]);

    if (exito == -1) {
        exit(1);
    }

    printf("El comando terminó de ejecutarse\n");
    
    return 0;
}