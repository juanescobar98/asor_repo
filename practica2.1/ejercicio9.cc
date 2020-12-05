#include <stdio.h> //printf()
#include <unistd.h> //id
#include <sys/types.h> //id
/*
    El ejercicio 8 es solo mirar la pagina de manual

    Si los idReal y idEfectivo son distintos, se puede asegurar que el bit setuid esta activado
*/
int main() {
    uid_t idReal = getuid();
    uid_t idEfectivo = geteuid();
    printf("Id real: %d\nId efectivo: %d\n", idReal, idEfectivo);
    return 0;
}
