#include <sys/utsname.h> //uname()
#include <stdio.h> //printf()
#include <errno.h> //errno
#include <string.h> //strerror()
/*
    El ejercicio 4 no tiene programa, es solo mirar la pagina de manual de uname(1)
*/
int main(){
    struct utsname informacion;
    int error = uname(&informacion);
    if (error == -1) {
        int idError = errno;
        printf("Codigo de error: %d\nsignificado: %s\n", idError, strerror(idError));
        return 1;
    }
    else
    {
        char *maquina = informacion.machine;
        char *nombreNodo = informacion.nodename;
        char *releaseNombre = informacion.release;
        char *sistema = informacion.sysname;
        char *versionSistema = informacion.version;
        printf("Hardware: %s\n Nombre del host: %s\n Release del SO: %s\n Nombre del SO: %s\n Version del SO: %s\n", maquina, nombreNodo, releaseNombre, sistema, versionSistema);
    }
    
    
    return 0;
}