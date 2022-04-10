//
// Created by xinya on 2022/4/9.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

void print_launch_info(FILE *fp, char *const argv[], int exit_value) {
    time_t raw_time;
    time(&raw_time);
    fprintf(fp, "<<========= watchdog exec (exit_value=%d):", exit_value);
    for (int i = 0; argv[i] != NULL; i++) fprintf(fp, " %s", argv[i]);
    fprintf(fp, " =========>> %s", ctime(&raw_time));
}

void launch(char *const argv[], const char *stdout_file, const char *stderr_file, int exit_value) {
    print_launch_info(stdout, argv, exit_value);

    if (stdout_file != NULL) {
        int fd = open(stdout_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd < 0) {
            perror("Cannot open stdout file");
            exit(exit_value);
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
        print_launch_info(stdout, argv, exit_value);
    }
    if (stderr_file != NULL) {
        int fd = open(stderr_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd < 0) {
            perror("Cannot open stderr file");
            exit(exit_value);
        }
        dup2(fd, STDERR_FILENO);
        close(fd);
        print_launch_info(stderr, argv, exit_value);
    }

    if (execvp(argv[0], argv) == -1) {
        perror("Cannot execute executable file");
        exit(exit_value);
    }
    // can never run here
}

void watchdog(char *const argv[], const char *stdout_file, const char *stderr_file, double interval, int exit_value) {
    while (1) {
        pid_t pid = fork();
        if (pid == 0) {
            launch(argv, stdout_file, stderr_file, exit_value);
            // can never run here
        }
        int status = 0;

        do {
            waitpid(pid, &status, 0);
        } while (!(WIFEXITED(status) || WIFSIGNALED(status)));
        if (WIFEXITED(status) && WEXITSTATUS(status) == exit_value) {
            exit(0);
        }
        usleep((int) (interval * 1e6));
    }
}

void help(const char *app) {
    printf("Usage: %s [options] executable [arguments]...\n", app);
    printf("\n");
    printf("    options:\n");
    printf("        --help:         print this help information.\n");
    printf("        --stdout:       redirect stdout.\n");
    printf("        --stderr:       redirect stderr.\n");
    printf("        --interval[0~]: interval before relaunch. [unit: s][default=1]\n");
    printf("        --exit[0~255]:  stop watchdog when specific exit value. [default=0]\n");
    printf("\n");
    printf("    executable:         the executable file.\n");
    printf("    arguments...:       arguments of the executable file.\n");
    exit(-1);
}

int main(int argc, char *argv[]) {
    const char *stdout_file = NULL;
    const char *stderr_file = NULL;
    double interval_ms = 1;
    int exit_value = 0;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--stdout") == 0 && ++i < argc) {
            stdout_file = argv[i];
        } else if (strcmp(argv[i], "--stderr") == 0 && ++i < argc) {
            stderr_file = argv[i];
        } else if (strcmp(argv[i], "--interval") == 0 && ++i < argc) {
            interval_ms = atof(argv[i]);
            if (interval_ms < 0) {
                fprintf(stderr, "invalid interval\n\n");
                help(argv[0]);
            }
        } else if (strcmp(argv[i], "--exit") == 0 && ++i < argc) {
            exit_value = atoi(argv[i]);
            if (!(0 <= exit_value && exit_value <= 255)) {
                fprintf(stderr, "invalid exit value\n\n");
                help(argv[0]);
            }
        } else if (strcmp(argv[i], "--help") == 0) {
            help(argv[0]);
        } else if (argv[i][0] == '-') {
            printf("No such option: '%s'\n\n", argv[i]);
            help(argv[0]);
        } else {
            watchdog(argv + i, stdout_file, stderr_file, interval_ms, exit_value);
            return -1;
        }
    }
    help(argv[0]);

    return 0;
}
