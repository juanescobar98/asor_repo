#include <sched.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>
#include <sys/time.h>
#include <sys/resource.h>

/*
Ej4:
$ ps aux
$ ps xao pid,pgid,sid,state,comm
el identificador es el del proceso que lo invoca

*/

int main(int argc, char **argv){
    pid_t pid, ppid, pgid, sid;

    pid = getpid();
    ppid = getppid();
    pgid = getpgid(pid);

    if (pgid == -1) {
        perror("getpgid");
        return 1;
    }
    
    sid = getsid(pid);

    if (sid == (pid_t) -1) {
        perror("getsid");
        return 1;
    }
    
    printf("PID: %ld  PPID: %ld  PGID: %ld  SID: %ld\n", pid, ppid, pgid, sid);

    char cwd[PATH_MAX], error[PATH_MAX];
    if(getcwd(cwd, sizeof(cwd)) == NULL){
        perror("getcwd");
        return 1;
    }

    printf("Directorio actual: %s\n", cwd);

    struct rlimit rlim;
    int exito;
    exito = getrlimit(RLIMIT_NOFILE, &rlim);
    if (exito == -1) {
        perror("getrlimit");
        return 1;
    }

    printf("El numero maximo de ficheros es: %ld\n", rlim.rlim_cur);
    

    return 0;
}