#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define MAX_ENVIO 256
#define MAX_CONEXIONES 3
/*
Servidor TCP concurrente
./ejercicio6 :: 3000
en cliente hacer:
nc -4 192.168.0.1 2222
o tambien
nc -6 fd00::a:0:0:0:1 2222


*/
int main(int argc, char **argv){

    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int error;
    int ds;
    char buffer[MAX_ENVIO] = "";
    struct sockaddr_storage dirHost;
    socklen_t dirHostLength;
    int bytes, enviados;
    dirHostLength = sizeof(struct sockaddr_storage);
    char envio[MAX_ENVIO];
    char host[NI_MAXHOST], port[NI_MAXSERV];
    int pid;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s nombre\n", argv[0]);
        exit(EXIT_FAILURE);
    }


    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM;          /* socket type, 0 indica cualquiera  */
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


    // int on = 0;
    // setsockopt(ds, IPPROTO_IPV6, IPV6_V6ONLY, (void *) &on, sizeof(on));

    error = listen(ds, MAX_CONEXIONES);
    if (error == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    int clisd, hijos;
    hijos = 0;

    while(buffer[0] != 'Q'){
        // printf("Esta en escucha\n");
        clisd = accept(ds, (struct sockaddr *)&dirHost, &dirHostLength);
        // printf("acepta\n");

        if (clisd == -1) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        
        
        pid = fork(); // nueva conexion -> nuevo hijo

        switch (pid)
        {
            case -1:
                perror("fork");
                close(clisd);
                close(ds);
                exit(EXIT_FAILURE);
                break;
            
            case 0:
                hijos = MAX_CONEXIONES;
                error = getnameinfo((struct sockaddr *) &dirHost, dirHostLength, host, NI_MAXHOST, port, NI_MAXSERV, NI_NUMERICHOST);
                if (error != 0) {
                    fprintf(stderr, "getnameinfo error\n");
                    exit(EXIT_FAILURE);
                }

                printf("Conexión desde %s:%s\n", host, port);

                bytes = recv(clisd, buffer, sizeof(buffer), 0);
                while(bytes != 0){ // si es 0 es que el cliente se ha cerrado y por tanto termina la conexion
                    if (bytes == -1) {
                        perror("error al recibir");
                        close(clisd);
                        close(ds);
                        exit(EXIT_FAILURE);
                    }
                    buffer[bytes] = '\0';
                    enviados = send(clisd, buffer, strlen(buffer), 0); //usamos strlen porque buffer ahora tiene un elemento mas
                    if (enviados == -1) {
                        perror("send");
                        close(clisd);
                        close(ds);
                        exit(EXIT_FAILURE);
                    }

                    if (!strcmp(buffer, "Q\n")) { // si el cliente envia Q acaba la ejecucion del servidor
                        close(clisd);
                        close(ds);
                        printf("Conexión terminada\n");
                        return 0;
                    }
                    
                    bytes = recv(clisd, buffer, sizeof(buffer), 0);
                }
                printf("Conexión terminada\n");
                close(clisd);
                exit(0); //el hijo termina
                //buffer[0] = 'Q'
                break;
        
            default:
                close(clisd);
                hijos++;
                break;
        }
        
        

        
        //close(clisd);
        
    }

    close(ds);

    return 0;
}
