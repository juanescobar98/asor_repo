#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/select.h>

#define MAX_ENVIO 256
#define HIJOS_MAX 3
/*
Dejo al padre activo para que funcione la entrada por consola tambien
Asi cuando se cierra al padre (ctrl+c) acaban los hijos tambien

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

    // int on = 0;
    // setsockopt(ds, IPPROTO_IPV6, IPV6_V6ONLY, (void *) &on, sizeof(on));

    fd_set conjunto;
    int seleccionado;
    int hijos = 0;
    int pid;
    while(hijos < HIJOS_MAX){
        
        pid = fork();
        switch (pid)
        {
            case 0:
                

                while(buffer[0] != 'q'){

                    FD_ZERO(&conjunto);
                    FD_SET(0, &conjunto);
                    FD_SET(ds, &conjunto);

                    seleccionado = select(ds + 1, &conjunto, NULL, NULL, NULL);

                    if (seleccionado == -1) {
                        perror("select");
                        exit(EXIT_FAILURE);
                    }
                    else if (seleccionado > 0)
                    {
                        if (FD_ISSET(0, &conjunto)) {
                            bytes = read(0, buffer, sizeof(buffer));
                            if (bytes == -1) {
                                perror("read");
                                exit(EXIT_FAILURE);
                            }
                            
                        }
                        else if (FD_ISSET(ds, &conjunto))
                        {
                            bytes = recvfrom(ds, buffer, sizeof(buffer), 0, (struct sockaddr *) &dirHost, &dirHostLength);
                            if (bytes == -1) {
                                continue;
                            }
                            
                            error = getnameinfo((struct sockaddr *) &dirHost, dirHostLength, host, NI_MAXHOST, port, NI_MAXSERV, NI_NUMERICHOST);
                            if (error != 0) {
                                fprintf(stderr, "getnameinfo error\n");
                                exit(EXIT_FAILURE);
                            }
                            
                            printf("%d bytes recibidos de %s:%s en el proceso: %d\n", bytes, host, port, getpid());
                        }
                        
                        
                    }
                    
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
                        
                        if (FD_ISSET(0, &conjunto)) {
                            printf("%s\n", envio);
                        }
                        else if (FD_ISSET(ds, &conjunto))
                        {
                            enviados = sendto(ds, envio, strlen(envio), 0, (struct sockaddr *) &dirHost, dirHostLength);
                            if (enviados == -1) {
                                perror("sendto");
                                exit(EXIT_FAILURE);
                            }
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
                    //sleep(10); // para ver los distintos procesos
                }
                hijos = HIJOS_MAX;
                break;
            
            case -1:
                perror("fork");
                exit(EXIT_FAILURE);
                break;
        
            default:
                hijos++;
                if (hijos == HIJOS_MAX) {
                    while(1){ // espera hasta que se cierre el servidor, si queremos hijos en 2o plano, quitar
                        
                    }
                    
                }
                
                break;
        }

    }
    


    close(ds);

    return 0;
}
