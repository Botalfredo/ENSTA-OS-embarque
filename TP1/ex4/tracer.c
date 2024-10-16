#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>

int main() {
    // Creation d'un fils avec la fonction fork
    pid_t child = fork();
    // Si le processus est le processus fils
    if (child == 0) {
        // On indique que le processus fils demande à être tracé (PTRACE_TRACEME) par son processus parent (0).
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        // execl remplace l'image du processus courant par un nouveau programme, ici ./fichier_ecriture.
        // le premier argument donne le chemin vers le programme, le second indique le nom tel qu'il apparait et NULL indique la fin des arguments.
        execl("./fichier_ecriture", "fichier_ecriture", NULL);
    } else {
        int status;
        struct user_regs_struct regs;
        int in_syscall = 0;

        while (1) {
            // waitpid attent que le processus fils change d'état. Cet état est sauvegardé dans status. Le 0 indique que le père attends n'import quel changement.
            waitpid(child, &status, 0);
            // On verfie que le processus fils c'est terminé normalement avec la macro WIFEXITED grâce au statut obetenu avec waitpid, sinon on kill
            if (WIFEXITED(status)) return 0;

            // ptrace avec PTRACE_GETREGS Récupère les registres du processus fils (précisé par child) et les stocke dans la structure regs.
            ptrace(PTRACE_GETREGS, child, NULL, &regs);
            // si regs.orig_rax contient le numéro de l'appel système intercepté SYS_write.
            if (regs.orig_rax == SYS_write) {
                // Si on est dans la première partie de l'appel système
                if (in_syscall == 0) {
                    // On passe dans la seconde partie de cet appel system
                    in_syscall = 1;
                    printf("=== Appel système write intercepté ===\n");
                    printf("Descripteur de fichier : %llu\n", regs.rdi);
                    printf("Adresse du buffer : %llx\n", regs.rsi);
                    printf("Taille : %llu\n", regs.rdx);
                } else {
                    // On reviens dans la première partie de l'appel système
                    in_syscall = 0;
                }
            }
            // ptrace avec PTRACE_SYSCALL demande au processus fils de s'arrêter à chaque entrée et sortie d'un appel système, 
            // permettant au processus parent de l'inspecter.
            ptrace(PTRACE_SYSCALL, child, NULL, NULL);
        }
    }
    return 0;
}
