#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/select.h>

/*

mkfifo prueba
mkfifo tuberia

*/

int main(int argc, char **argv){

    if (argc > 1) {
        fprintf(stderr, "Uso: ./ejercicio5\n");
        exit(EXIT_FAILURE);
    }

    int error, prueba, tuberia, seleccionado;

    prueba = open("prueba", O_RDONLY | O_NONBLOCK);

    if (prueba == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    tuberia = open("tuberia", O_RDONLY | O_NONBLOCK);

    if (tuberia == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    //buffer
    char buffer[256];
    fd_set conjunto;

    int maximo, existePrueba, existeTuberia, actual, tamLectura;
    seleccionado = 0;

    while(seleccionado != -1){

        if (prueba > tuberia) {
            maximo = prueba + 1;
        }
        else
        {
            maximo = tuberia + 1;
        }

        FD_ZERO(&conjunto);
        FD_SET(prueba, &conjunto);
        FD_SET(tuberia, &conjunto);
        
        seleccionado = select(maximo, &conjunto, NULL, NULL, NULL);

        if (seleccionado > 0) {
            
            existePrueba = FD_ISSET(prueba, &conjunto);
            existeTuberia = FD_ISSET(tuberia, &conjunto);
            if (existePrueba) {
                actual = prueba;
            }
            else if (existeTuberia)
            {
                actual = tuberia;
            }
            
            //leer y escribir
            
            tamLectura = 255;

            while(tamLectura == 255){ //mientras queden bytes por leer
                tamLectura = read(actual, buffer, 255);

                if (tamLectura == -1) {
                    perror("read");
                    exit(EXIT_FAILURE);
                }
                                    
                
                buffer[tamLectura] = '\0';

                if (tamLectura > 1) { //el salto de linea cuenta como elemento, si solo hay uno es ese
                    if (existePrueba) {
                        printf("Prueba: %s", buffer);
                    }
                    else
                    {
                        printf("Tuberia: %s", buffer);
                    }
                }

                if (tamLectura == 255) { //si quedan mas por leer se deja espacio para separar los mensajes
                    printf("\n");
                }
                
            }
            
            
            
            

            //cerrar y abrir

            error = close(actual);
            if (error == -1) {
                perror("close");
                exit(EXIT_FAILURE);
            }

            if (actual == prueba) {
                prueba = open("prueba", O_RDONLY | O_NONBLOCK);

                if (prueba == -1) {
                    perror("open");
                    close(tuberia);
                    exit(EXIT_FAILURE);
                }
            }
            else
            {
                tuberia = open("tuberia", O_RDONLY | O_NONBLOCK);

                if (tuberia == -1) {
                    perror("open");
                    close(prueba);
                    exit(EXIT_FAILURE);
                }
            }
            
            

        }
        
    }
    
    
    perror("Ha ocurrido un error al seleccionar el pipe, fin del programa");

    
    error = close(prueba);
    if (error == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }

    error = close(tuberia);
    if (error == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }

    return 0; // return -1 seria mas correcto
}