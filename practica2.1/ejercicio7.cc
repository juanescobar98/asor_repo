#include <unistd.h> //pathconf
#include <stdio.h> //printf()
#include <string.h> //strerror()
#include <stdio.h> //perror

int main(){
    
    int valor = pathconf(".", _PC_LINK_MAX); // . actual, / para raiz, da igual cual poner en el ejemplo
    if (valor == -1) {
        perror("pathconf('.',_PC_LINK_MAX)");
        return 1;
    }

    printf("El numero maximo de enlaces es %ld\n", valor);
    
    valor = pathconf(".",_PC_PATH_MAX);
    if (valor == -1) {
        perror("pathconf('.',_PC_PATH_MAX)");
        return 1;
    }

    printf("La longitud maxima de la ruta es %ld\n", valor);

       valor = pathconf(".",_PC_NAME_MAX);
    if (valor == -1) {
        perror("pathconf(_PC_NAME_MAX)");
        return 1;
    }

    printf("La longitud maxima de nombre de archivo es %ld\n", valor);
    
    
    return 0;
}