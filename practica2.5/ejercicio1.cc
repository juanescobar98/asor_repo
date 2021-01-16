#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUF_SIZE 256

/*
Pagina de manual de getaddrinfo(3)
*/
int main(int argc, char **argv){


    if (argc != 2) {
        fprintf(stderr, "Usage: %s nombre\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct addrinfo hints;
    struct addrinfo *result, *rp;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = 0; /* Datagram socket, 0 indica cualquiera  */
    hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
    hints.ai_protocol = 0;          /* Any protocol */
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;


    int error;
    
    error = getaddrinfo(argv[1], NULL, &hints, &result);
    if (error != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(error));
        exit(EXIT_FAILURE);
    }

    printf("|||||Lista de sockets|||||\n");
    for (rp = result; rp != NULL; rp = rp->ai_next){

        char host[NI_MAXHOST], server[NI_MAXSERV];
        error = getnameinfo(rp->ai_addr, rp->ai_addrlen, host, NI_MAXHOST, server, NI_MAXSERV, NI_NUMERICHOST);
        if (error != 0) {
            fprintf(stderr, "getnameinfo error\n");
        }
        printf("%s\t%d\t%d\n",host, rp->ai_family, rp->ai_socktype);
        
    }


    return 0;
}
