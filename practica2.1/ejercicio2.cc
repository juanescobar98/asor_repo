#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int main() {
    int id = setuid(0);
    if(id == -1){
        int idError = errno;
        perror("setuid");
        printf("Codigo de error: %d\nsignificado: %s\n", idError, strerror(idError));
        return 1;
    }
    return 0;
}
