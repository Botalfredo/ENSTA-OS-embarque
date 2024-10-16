#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *f = fopen("output.txt", "w");
    fprintf(f, "Hello, World!\n");
    fclose(f);
    return 0;
}
