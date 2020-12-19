/*
    Ej8: man de stat

    Ej9: st_mtime no tiene en cuenta las modificaciones de permisos del fichero, enlaces o modos
    st_ctime si
*/

#include <sys/types.h>
#include <sys/stat.h>
//#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
       #include <stdlib.h>
       #include <sys/sysmacros.h>

int main(int argc, char **argv){
    /* mode_t mascara = umask(0027);
    int fichero = open("ficheroEj7.txt", O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH | S_IXOTH);
    if (fichero == -1) {
        perror("open");
        return 1;
    }
    
    //la mascara en shell sigue siendo 0002 si no se ha cambiado */
    /* struct stat buffer; */

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

           printf("File type:                ");

               switch (sb.st_mode & S_IFMT) {
           case S_IFBLK:  printf("block device\n");            break;
           case S_IFCHR:  printf("character device\n");        break;
           case S_IFDIR:  printf("directory\n");               break;
           case S_IFIFO:  printf("FIFO/pipe\n");               break;
           case S_IFLNK:  printf("symlink\n");                 break;
           case S_IFREG:  printf("regular file\n");            break;
           case S_IFSOCK: printf("socket\n");                  break;
           default:       printf("unknown?\n");                break;
           }

           printf("I-node number:            %ld\n", (long) sb.st_ino);
/* 
           printf("Mode:                     %lo (octal)\n",
                   (unsigned long) sb.st_mode);

           printf("Link count:               %ld\n", (long) sb.st_nlink);
           printf("Ownership:                UID=%ld   GID=%ld\n",
                   (long) sb.st_uid, (long) sb.st_gid);

           printf("Preferred I/O block size: %ld bytes\n",
                   (long) sb.st_blksize);
           printf("File size:                %lld bytes\n",
                   (long long) sb.st_size);
        printf("Blocks allocated:         %lld\n",
                   (long long) sb.st_blocks); */

           //printf("Last status change:       %s", ctime(&sb.st_ctime));
           printf("Last file access:         %s", ctime(&sb.st_atime));
           //printf("Last file modification:   %s", ctime(&sb.st_mtime));

           exit(EXIT_SUCCESS);



    return 0;
}