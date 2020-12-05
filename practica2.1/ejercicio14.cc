#include <stdio.h> //printf()
#include <time.h> //localtime
/*

*/
int main() {

    time_t tiempo = time( NULL ); 
    //tambien se puede hacer con time(NULL) pero no guarda el resultado en la variable segundos

    if (tiempo == -1) {
        perror("time( &(hora.tv_sec) )");
        return 1;
    }
    struct tm *valor = localtime(&tiempo);
    if (valor == NULL) {
        perror("gettimeofday(&horaAntes, NULL)");
        return 1;
    }
    printf("Tiempo en años : %d\n", 1900 + valor->tm_year); //1900 es el primer anio
    return 0;
}
