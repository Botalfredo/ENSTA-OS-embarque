#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Usage: %s <fichierAEcrire> <nombre de blocks>\n", argv[0]);
        exit(0);
    }

    char *fichierAEcrire = argv[1];
    char *nombreDeBlocks = argv[2];

    char *cmd[] = {
        "dd",
        "if=/dev/urandom",
        NULL,
        NULL,
        NULL,
        NULL
    };

    char ofArgument[256];
    char countArgument[256];

    snprintf(ofArgument, sizeof(ofArgument), "of=%s", fichierAEcrire);
    snprintf(countArgument, sizeof(countArgument), "count=%s", nombreDeBlocks);

    cmd[2] = ofArgument;
    cmd[3] = "bs=4096";
    cmd[4] = countArgument;

    execvp("dd", cmd);

    perror("exec");
    return EXIT_FAILURE;
}
