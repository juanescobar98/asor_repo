#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysmacros.h>
#include <time.h> //localtime
#include <locale.h> //setlocale
#include <string.h>
#include <sys/types.h>
#include <dirent.h>


int main(int argc, char **argv){

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <pathname directory>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int error;
    int sizeOfFiles = 0;
    DIR * directorio;
    directorio = opendir(argv[1]);
    if (directorio == NULL) {
        perror("opendir");
        return 1;
    }

    struct dirent *entrada;
    struct stat sb;
    char simLinkPath[PATH_MAX];
    while ((entrada = readdir(directorio)) != NULL) {
        lstat(entrada->d_name, &sb);      
        

        switch (sb.st_mode & S_IFMT) {
            //case S_IFBLK:  printf("block device\n");            break;
            //case S_IFCHR:  printf("character device\n");        break;
            case S_IFDIR:  printf("%s/", entrada->d_name);               break;
            //case S_IFIFO:  printf("FIFO/pipe\n");               break;
            case S_IFLNK:  
                readlink(entrada->d_name,simLinkPath, sizeof(simLinkPath) - 1); 
                printf("%s -> %s", entrada->d_name, simLinkPath);
                break;
            case S_IFREG:  printf("%s", entrada->d_name);            break;
            //case S_IFSOCK: printf("socket\n");                  break;
            default:       printf("unknown?");                break;
        } 
        if (access(entrada->d_name, X_OK)) {
            printf("\n");
        } else {
            printf("*\n");
        }        
    }

    error = closedir(directorio);

    if (error == -1) {
        perror("closedir");
        return 1;
    }
    
    return 0;
}