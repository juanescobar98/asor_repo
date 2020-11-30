#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    int id = setuid(0);
    if(id == -1){
        perror("setuid");
        return 1;
    }
    return 0;
}
