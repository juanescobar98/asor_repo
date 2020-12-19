/*
    Ej14: Mirar fichero /proc/locks
*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysmacros.h>
#include <time.h> //localtime
#include <locale.h> //setlocale
#include <string.h>

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
    
    error = lockf(fichero, F_TLOCK, 0);

    if (error == -1) {
        printf("El fichero ya esta bloqueado\n");
        return 0;
    }
    else {
        printf("Fichero bloqueado, por favor, espere\n");

        setlocale(LC_ALL, "es_ES");
        time_t tiempo;
        tiempo = time( NULL ); 
        if (tiempo == -1) {
            perror("time");
            return 1;
        }

        char resultado[200];
        struct tm *valor = localtime(&tiempo);
        if (valor == NULL) {
            perror("gettimeofday(&horaAntes, NULL)");
            return 1;
        }
        strftime(resultado, 200, "%A, %d de %B de %Y, %H:%I", valor);
        strcat(resultado, "\n");

        write(fichero, resultado, strlen(resultado));

        sleep(30); //Tiempo en segundos

        error = lockf(fichero, F_ULOCK, 0);

        if (error == -1) {
            perror("El fichero no se ha desbloqueado\n");
            return 0;
        }

        printf("Fichero  desbloqueado\n");
    }
            
    

    


    return 0;
}