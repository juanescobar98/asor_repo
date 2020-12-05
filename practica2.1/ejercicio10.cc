#include <stdio.h> //printf()
#include <unistd.h> //id
#include <sys/types.h> //id pw
#include <pwd.h> //pw

/*
    Para getpwuid -> id de getuid
    Para getpwnam -> nombre de usuario (p.e. "root")
*/
int main() {
    uid_t idReal = getuid();
    uid_t idEfectivo = geteuid();
    
    printf("Id real: %d\nId efectivo: %d\n", idReal, idEfectivo);

    struct passwd *infoReal = getpwuid(idReal);

    if (infoReal == NULL) {
        perror("getpwuid(idReal)");
        return 1;
    }
    
    struct passwd *infoEfectivo = getpwuid(idEfectivo);

    if (infoEfectivo == NULL) {
        perror("getpwuid(idEfectivo)");
        return 1;
    }

    printf("Nombre real: %s\nNombre efectivo: %s\n", infoReal->pw_name, infoEfectivo->pw_name);
    printf("Directorio home real: %s\nDirectorio home efectivo: %s\n", infoReal->pw_dir, infoEfectivo->pw_dir);
    printf("Descripcion real: %s\nDescripcion efectivo: %s\n", infoReal->pw_gecos, infoEfectivo->pw_gecos);


    return 0;
}
