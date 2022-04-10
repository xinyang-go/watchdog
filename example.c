#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void help(const char *app) {
    printf("Usage: %s [flag]\n", app);
    printf("\n");
    printf("    flag:\n");
    printf("        --zero-exit\n");
    printf("        --nonzero-exit\n");
    printf("        --segmentation-fault\n");
    exit(-1);
}

int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--zero-exit") == 0) {
            fprintf(stdout, "example stdout.\n");
            fprintf(stderr, "example stderr.\n");
            return 0;
        } else if (strcmp(argv[i], "--nonzero-exit") == 0) {
            fprintf(stdout, "example stdout.\n");
            fprintf(stderr, "example stderr.\n");
            return -1;
        } else if (strcmp(argv[i], "--segmentation-fault") == 0) {
            fprintf(stdout, "example stdout.\n");
            fprintf(stderr, "example stderr.\n");
            fflush(stdout);
            (*(volatile int *) NULL) = 0;
        } else {
            help(argv[0]);
        }
    }
    help(argv[0]);
}
