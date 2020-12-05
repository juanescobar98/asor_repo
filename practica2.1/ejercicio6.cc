#include <unistd.h> //sysconf
#include <stdio.h> //printf()
#include <string.h> //strerror()
#include <stdio.h> //perror
/*
    
*/
int main(){
    
    int valor = sysconf(_SC_ARG_MAX);
    if (valor == -1) {
        perror("sysconf(_SC_ARG_MAX)");
        return 1;
    }

    printf("El numero maximo de argumentos es %ld\n", valor);
    
    valor = sysconf(_SC_CHILD_MAX);
    if (valor == -1) {
        perror("sysconf(_SC_CHILD_MAX)");
        return 1;
    }

    printf("El numero maximo de hijos es %ld\n", valor);

       valor = sysconf(_SC_OPEN_MAX);
    if (valor == -1) {
        perror("sysconf(_SC_OPEN_MAX)");
        return 1;
    }

    printf("El numero maximo de ficheros abiertos es %ld\n", valor);
    
    
    return 0;
}