#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

/*
No se muestra el mensaje final porque este comando cambia la imagen del proceso por uno nuevo
*/


int main(int argc, char **argv){
    
    int exito;

    if (argc < 2) {
        printf("Uso: ./ejercicio7b nombrePrograma arg1 arg2 ... \n");
        exit(1);
    }
    
    exito = execvp(argv[1], argv+1); 
    //argv + 1 suma lo que ocupa el primer elemento, es decir, el nombre del programa para obtener el vector de parametros

    if (exito == -1) {
        exit(1);
    }

    printf("El comando terminó de ejecutarse\n");
    
    return 0;
}