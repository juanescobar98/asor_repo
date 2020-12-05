#include <string.h>
#include <stdio.h>

int main(){
    //originalmente hasta 255 pero realmente hay 134 errores conocidos, contando el valor 0 = success. 
    for(int i = 0; i < 134; i++)
    {
        printf("Codigo de error: %d\nsignificado: %s\n", i, strerror(i));
    }
    return 0;
}