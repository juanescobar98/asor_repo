#include <stdio.h> //printf()
#include <time.h> //time
#include <sys/time.h> //gettimeofday
/*

*/
int main() {

    struct timeval horaAntes, horaDespues;
    int valor = gettimeofday(&horaAntes, NULL);
    for(int i = 0; i < 1000000; i++);
    int valor2 = gettimeofday(&horaDespues, NULL);
    if (valor == -1) {
        perror("gettimeofday(&horaAntes, NULL)");
        return 1;
    }
    if (valor2 == -1) {
        perror("gettimeofday(&horaDespues, NULL)");
        return 1;
    }
    time_t segundosAntes = horaAntes.tv_usec;
    time_t segundosDespues = horaDespues.tv_usec;
    time_t segundosTranscurridos = segundosDespues - segundosAntes;
    //tambien se puede hacer con time(NULL) pero no guarda el resultado en la variable segundos

    printf("Tiempo inicio (ms): %ld\n", segundosAntes);
    printf("Tiempo fin (ms): %ld\n", segundosDespues);
    printf("Tiempo transcurrido (ms): %ld\n", segundosTranscurridos);
    return 0;
}
