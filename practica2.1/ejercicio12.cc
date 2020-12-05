#include <stdio.h> //printf()
#include <time.h> //time
#include <sys/time.h> //gettimeofday
/*
    El ejercicio 11 es de mirar las paginas de manual.
    Epoch es la medianoche del 1 de enero de 1970.
    Las llamadas devuelven -1 si fallo y dan valor a errno.
*/
int main() {

    struct timeval hora;
    int valor = gettimeofday(&hora, NULL);

    if (valor == -1) {
        perror("gettimeofday(&hora, NULL)");
        return 1;
    }

    time_t segundos = hora.tv_sec;
    long tiempo = time( &segundos ); 
    //tambien se puede hacer con time(NULL) pero no guarda el resultado en la variable segundos

    if (tiempo == -1) {
        perror("time( &(hora.tv_sec) )");
        return 1;
    }
    
    printf("Hora en segundos desde Epoch: %ld\n", tiempo);
    return 0;
}
