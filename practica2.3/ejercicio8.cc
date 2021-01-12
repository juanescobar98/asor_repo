#include <sched.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

/*

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

int ejecucion(int argc, char **argv){
    int exito;

    if (argc < 2) {
        fprintf(stderr, "Uso: ./ejercicio8 nombrePrograma arg1 arg2 ... \n"); //stderr = salida de error
        exit(1);
    }
    
    exito = execvp(argv[1], argv+1); 
    
    if (exito == -1) {
        exit(1);
    }

    printf("El comando terminó de ejecutarse\n");
    
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


        int error, ficheroEnt, ficheroSal, ficheroErr;
        ficheroEnt = open("/dev/null", O_CREAT | O_WRONLY, 0777);

        if (ficheroEnt == -1) {
            perror("open ent");
            return 1;
        }
        error = dup2(ficheroEnt, 0);
        if (error == -1) {
            perror("dup2(ficheroEnt, 0)");
            return 1;
        }

        ficheroSal = open("/tmp/daemon.out", O_CREAT | O_WRONLY, 0777);

        if (ficheroSal == -1) {
            perror("open sal");
            return 1;
        }
        error = dup2(ficheroSal, 1);
        if (error == -1) {
            perror("dup2(ficheroSal, 1)");
            return 1;
        }

        ficheroErr = open("/tmp/daemon.err", O_CREAT | O_WRONLY, 0777);

        if (ficheroErr == -1) {
            perror("open sal");
            return 1;
        }
        error = dup2(ficheroErr, 2);
        if (error == -1) {
            perror("dup2(ficheroErr, 1)");
            return 1;
        }

        infoProceso("Hijo");
        
        ejecucion(argc, argv);        
    }
    else
    {
        // sleep(5);
        infoProceso("Padre");
    }
    return 0;
}