#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>

#define ORIG_RAX 15 // Offset pour x86_64

int main() {
    // Creation d'un fils avec la fonction fork
    pid_t child = fork();
    // Si le processus est le processus fils
    if (child == 0) {
        // On indique que le processus fils demande à être tracé (PTRACE_TRACEME) par son processus parent (0).
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        // execl remplace l'image du processus courant par un nouveau programme, ici ./fichier_ecriture.
        // le premier argument donne le chemin vers le programme, le second indique le nom tel qu'il apparait et NULL indique la fin des arguments.
        execl("/bin/ls", "ls", NULL);
    } else {
        int status;
        // waitpid attent que le processus fils change d'état. Cet état est sauvegardé dans status. Le 0 indique que le père attends n'import quel changement.
        waitpid(child, &status, 0);
        // On verfie que le processus fils c'est terminé normalement avec la macro WIFEXITED grâce au statut obetenu avec waitpid
        if (WIFEXITED(status)) return 0;

        // ptrace avec PTRACE_PEEKUSER lit la valeur d'un registre du processus fils (ici ORIG_RAX), qui contient le numéro de l'appel système exécuté.
        // sizeof(long) * ORIG_RAX permet de calculer le décalage jusqu'a l'information de ORIG_RAX pour enuite le stocker dans orig_rax
        long orig_rax = ptrace(PTRACE_PEEKUSER, child, sizeof(long) * ORIG_RAX, NULL);
        printf("Le processus fils a exécuté un appel système : %ld\n", orig_rax);
        // ptrace avec PTRACE_CONT Demande au processus fils de continuer son exécution après l'interception de l'appel système.
        ptrace(PTRACE_CONT, child, NULL, NULL);
        // Attend un changement d'état du processus fils, comme la fin de l'appel système pour permettre de le tracer après qu'il ai repris sont execution.
        waitpid(child, &status, 0);
    }
    return 0;
}
