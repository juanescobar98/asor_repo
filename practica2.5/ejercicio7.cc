#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAX_RECIBIDO 256
/*
Cliente TCP
uso:
./ejercicio7 fd00::a:0:0:0:1 2222
./ejercicio7 192.168.0.1 2222

*/
int main(int argc, char **argv){

    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int error;
    int ds;
    char buffer[MAX_RECIBIDO] = "";
    //struct sockaddr_storage dirHost;
    //socklen_t dirHostLength;
    int bytes, enviados;
    //dirHostLength = sizeof(struct sockaddr_storage);
    //char host[NI_MAXHOST], port[NI_MAXSERV];

    if (argc != 3) {
        fprintf(stderr, "Usage: %s direccion puerto\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM;          /* socket type, 0 indica cualquiera  */
    hints.ai_flags = 0;    /* For wildcard IP address */
    hints.ai_protocol = 0;          /* Any protocol */
    
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
        else
        {
            break;
        }    
    }

    if (rp == NULL) {
        fprintf(stderr, "Error, no se pudo establecer una direccion local para el socket\n");
        exit(EXIT_FAILURE);
    }

    error = connect(ds, (struct sockaddr *)rp->ai_addr, (rp->ai_addrlen));
    if(error == -1) {
        perror("connect");
        close(ds);
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(result);
    // int on = 0;
    // setsockopt(ds, IPPROTO_IPV6, IPV6_V6ONLY, (void *) &on, sizeof(on));

    while(strcmp(buffer, "Q\n")){

        bytes = read(0, buffer, sizeof(buffer));
        if (bytes == -1) {
            perror("read");
            close(ds);
            exit(EXIT_FAILURE);
        }

        buffer[bytes] = '\0';
        enviados = send(ds, buffer, strlen(buffer), 0);
        if (enviados == -1) {
            perror("sendto");
            close(ds);
            exit(EXIT_FAILURE);
        }    

        bytes = recv(ds, buffer, sizeof(buffer), 0);
        if (bytes == -1) {
            fprintf(stderr, "error al recibir el mensaje\n");
        }
        buffer[bytes] = '\0';
        printf("%s", buffer);
    }


    close(ds);

    return 0;
}
