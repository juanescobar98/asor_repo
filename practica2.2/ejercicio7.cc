/*
    Ej6: umask 027
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(){
    mode_t mascara = umask(0027);
    int fichero = open("ficheroEj7.txt", O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH | S_IXOTH);
    if (fichero == -1) {
        perror("open");
        return 1;
    }
    
    //la mascara en shell sigue siendo 0002 si no se ha cambiado

    return 0;
}