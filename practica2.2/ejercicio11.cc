/*
    Ej10: 
    simbolico -> distinto i-nodo, no aumenta el nº de enlaces
    si se borra, no pasa nada, si se borra el original, se pierde la informacion

    rigido -> mismo i-nodo, aumenta el nº enlaces en el fichero original
    si se borra, se decrementa pero mantiene informacion
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/sysmacros.h>
#include <string.h>

int main(int argc, char **argv){
    struct stat sb;

           if (argc != 2) {
               fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
               exit(EXIT_FAILURE);
           }

           if (stat(argv[1], &sb) == -1) {
               perror("stat");
               exit(EXIT_FAILURE);
           }

           printf("ID of containing device (major, minor):  [%li,%li]\n",
                  (long) major(sb.st_dev), (long) minor(sb.st_dev));

               switch (sb.st_mode & S_IFMT) {
           case S_IFREG:
                // copiamos el nombre del archivo y le añadimos la extension .sym y .hard para cada enlace
                char simName[200];
                char hardName[200];

                strcpy(simName, argv[1]);
                strcpy(hardName, argv[1]);
                strcat(simName, ".sym");
                strcat(hardName, ".hard");

                int error;
                error = link(argv[1], hardName); //success = 0
                if (error == -1) {
                    perror("link");
                    return 1;
                }
                
                error = symlink(argv[1], simName); //success = 0
                if (error == -1) {
                    perror("symlink");
                    return 1;
                }

                break;

           default:
           break;
           }




    return 0;
}