#include <sched.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdlib.h>

/*
Si el padre termina antes, el hijo es adoptado por otro proceso padre (ppid=1) el init.
El control de la consola se devuelve antes de que termine el proceso hijo, no se deberia escribir en ella como en el ejemplo

Si el hijo termina antes, el ppid sigue siendo el del proceso padre, y se queda en estado zombie hasta que termina el padre.

*/

int infoProceso(char *nombre){
    pid_t pid, ppid, pgid, sid;

    if (nombre == NULL) {
        printf("error, nombre no definido\n");
        exit(-1);
    }
    
    printf("Proceso %s----------\n", nombre);

    pid = getpid();
    ppid = getppid();
    pgid = getpgid(pid);

    if (pgid == -1) {
        perror("getpgid");
        exit(-1);
    }
    
    sid = getsid(pid);

    if (sid == (pid_t) -1) {
        perror("getsid");
        exit(-1);
    }
    
    printf("PID: %ld  PPID: %ld  PGID: %ld  SID: %ld\n", pid, ppid, pgid, sid);

    char cwd[PATH_MAX], error[PATH_MAX];
    if(getcwd(cwd, sizeof(cwd)) == NULL){
        perror("getcwd");
        exit(-1);
    }

    printf("Directorio actual: %s\n", cwd);

    struct rlimit rlim;
    int exito;
    exito = getrlimit(RLIMIT_NOFILE, &rlim);
    if (exito == -1) {
        perror("getrlimit");
        exit(-1);
    }

    printf("El numero maximo de ficheros es: %ld\n", rlim.rlim_cur);
    

    return 0;
}

int main(int argc, char **argv){
    
    pid_t pid;

    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(1);
    }
    else if (pid == 0)
    {
        // sleep(3);
        pid_t sid;
        sid = setsid();
        if (sid == (pid_t)-1) {
            perror("setsid");
            exit(-1);
        }

        int exito;
        exito = chdir("/tmp");
        if (exito == -1) {
            perror("chdir");
            exit(-1);
        }

        infoProceso("Hijo");        
    }
    else
    {
        // sleep(20);
        infoProceso("Padre");
    }
    return 0;
}