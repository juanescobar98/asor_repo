#include <stdio.h> //printf()
#include <time.h> //localtime
#include <locale.h> //setlocale
/*

*/
int main() {

    setlocale(LC_ALL, "es_ES");
    time_t tiempo = time( NULL ); 
    //tambien se puede hacer con time(NULL) pero no guarda el resultado en la variable segundos

    if (tiempo == -1) {
        perror("time( &(hora.tv_sec) )");
        return 1;
    }

    char *resultado;
    struct tm *valor = localtime(&tiempo);
    if (valor == NULL) {
        perror("gettimeofday(&horaAntes, NULL)");
        return 1;
    }
    strftime(resultado, 200, "%A, %d de %B de %Y, %H:%I", valor);
    printf("%s\n", resultado);
    return 0;
}
