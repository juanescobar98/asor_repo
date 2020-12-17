/*
    Ej1:
    ls -a muestra todas las entradas, sin omitir las que comienzan por "."
    ls -l usa un formato largo para escribir la informacion
    ls -d lista directorios, no sus contenidos
    ls -h se usa con -l para ver el tamaño de forma visible para los humanos (ej 1K)
    ls -i (--inode) pone el numero del i-nodo de cada archivo
    ls -R lista subdirectorios de forma recursiva
    ls -1 lista un archivo por linea
    ls -F (--clasify) pone un indicador a cada entrada
    ls --color da color a la salida

    Ej2: solo ver
    
    Ej3: rw-r--r-x
    chmod 644 fichero
    chmod u+rw-x,g+r-wx,o+rx-w fichero

    Ej4: chmod u-x,g-x,o-x dirEj4
    [cursoredes@localhost Documents]$ cd dirEj4/
    bash: cd: dirEj4/: Permission denied

    
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(){

    int fichero = open("ficheroEj5.txt", O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH | S_IXOTH);
    if (fichero == -1) {
        perror("open");
        return 1;
    }
    


    return 0;
}