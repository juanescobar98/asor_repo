#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define MAX_ENVIO 256
/*
Servidor UDP
./time_server :: 3000
en cliente hacer:
nc -u 192.168.0.1 3000
o tambien
nc -u fd00::a:0:0:0:1 3000

*/
int main(int argc, char **argv){

    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int error;
    int ds;
    char buffer[2] = "";
    struct sockaddr_storage dirHost;
    socklen_t dirHostLength;
    int bytes, enviados;
    dirHostLength = sizeof(struct sockaddr_storage);
    char envio[MAX_ENVIO];
    char host[NI_MAXHOST], port[NI_MAXSERV];

    if (argc != 3) {
        fprintf(stderr, "Usage: %s nombre\n", argv[0]);
        exit(EXIT_FAILURE);
    }


    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_DGRAM;          /* socket type, 0 indica cualquiera  */
    hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
    hints.ai_protocol = 0;          /* Any protocol */
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;


    
    error = getaddrinfo(argv[1], argv[2], &hints, &result);
    if (error != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(error));
        exit(EXIT_FAILURE);
    }


    for (rp = result; rp != NULL; rp = rp->ai_next){
        ds = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

        if (ds == -1) {
            perror("socket");
            continue;
        }
        
        error = bind(ds, rp->ai_addr, rp->ai_addrlen);
        if (error == 0) {
            break; //si se consigue hacer bind sale del bucle
        }
        
        close(ds);        
    }

    if (rp == NULL) {
        fprintf(stderr, "Error, no se pudo establecer una direccion local para el socket\n");
        exit(EXIT_FAILURE);
    }
    
    freeaddrinfo(result); //no necesitamos la lista de direcciones


    while(buffer[0] != 'q'){
        bytes = recvfrom(ds, buffer, sizeof(buffer), 0, (struct sockaddr *) &dirHost, &dirHostLength);
        if (bytes == -1) {
            continue;
        }
        
        error = getnameinfo((struct sockaddr *) &dirHost, dirHostLength, host, NI_MAXHOST, port, NI_MAXSERV, NI_NUMERICHOST);
        if (error != 0) {
            fprintf(stderr, "getnameinfo error\n");
            exit(EXIT_FAILURE);
        }

        printf("%d bytes recibidos de %s:%s\n", bytes, host, port);

        if (buffer[0] == 't' || buffer[0] == 'd') {

            time_t tiempo = time( NULL ); 
            struct tm *valor = localtime(&tiempo);
            if (valor == NULL) {
                perror("localtime");
                return 1;
            }

            if (buffer[0] == 't') {
                
                strftime(envio, MAX_ENVIO, "%I:%M:%S %p", valor);                
            }
            else if (buffer[0] == 'd')
            {
                strftime(envio, MAX_ENVIO, "%Y-%m-%d", valor);
            }
            
            enviados = sendto(ds, envio, strlen(envio), 0, (struct sockaddr *) &dirHost, dirHostLength);
            if (enviados == -1) {
                perror("sendto");
                exit(EXIT_FAILURE);
            }
        }
        else if (buffer[0] == 'q')
        {
            printf("Saliendo...\n");
            break;
        }
        else
        {
            printf("Comando no soportado: %c\n", buffer[0]);
        }
        
    }

    close(ds);

    return 0;
}
