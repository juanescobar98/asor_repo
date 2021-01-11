#include <sched.h>
#include <stdio.h>


/*
Ej1:
renice -n -10 -p 7035 // 7035 es el pid de bash de la shell
chrt -f -p 12 7035 // 12 es la prioridad

Ej3:
La politica de privacidad es: SCHED_FIFO
La prioridad del proceso es 12
La prioridad maxima es 99
La prioridad minima es 1

Sí, se heredan los atributos del programa, es 12 la prioridad

*/

int main(int argc, char **argv){
    int politica;
    politica = sched_getscheduler(0); //0 es el proceso actual

    //Gestion de errores
    if (politica == -1) {
        perror("sched_getscheduler");
        return 1;
    }
    
    printf("La politica de privacidad es: ");

    switch (politica)
    {
        case SCHED_OTHER:
            printf("SCHED_OTHER");
            break;
        case SCHED_FIFO:
            printf("SCHED_FIFO");
            break;
        case SCHED_RR:
            printf("SCHED_RR");
            break;
        default:
            printf("planificador desconocido");
            break;
    }
    printf("\n");

    struct sched_param parametros;
    int minimo, maximo, prioridad, exito;

    exito = sched_getparam(0, &parametros);

    if (exito == -1) {
        perror("sched_getparam");
        return 1;
    }

    prioridad = parametros.__sched_priority;

    maximo = sched_get_priority_max(politica);

    if (maximo == -1) {
        perror("sched_get_priority_max");
        return 1;
    }

    minimo = sched_get_priority_min(politica);

    if (minimo == -1) {
        perror("sched_get_priority_min");
        return 1;
    }
    
    printf("La prioridad del proceso es %i\n", prioridad);
    printf("La prioridad maxima es %i\n", maximo);
    printf("La prioridad minima es %i\n", minimo);

    return 0;
}