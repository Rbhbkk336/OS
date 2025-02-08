#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>

void print_time() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    struct tm *tm = localtime(&ts.tv_sec);
    printf("%02d:%02d:%02d:%03ld", tm->tm_hour, tm->tm_min, tm->tm_sec, ts.tv_nsec / 1000000);
}

int main() {
    pid_t pid1, pid2;

    pid1 = fork();
    if (pid1 == 0) {
        printf("Child process: pid=%d, ppid=%d, time: ", getpid(), getppid());
        print_time();
        printf("\n");
        exit(0);
    } else if (pid1 > 0) {
        pid2 = fork();
        if (pid2 == 0) {
            printf("Child process: pid=%d, ppid=%d, time: ", getpid(), getppid());
            print_time();
            printf("\n");
            exit(0);
        } else if (pid2 > 0) {
            printf("Parent process: pid=%d, time: ", getpid());
            print_time();
            printf("\n");

            system("ps -x | grep '1.exe'");
        } else {
            perror("Fail to second fork()");
            exit(1);
        }
    } else {
        perror("Fail to first fork()");
        exit(1);
    }

    return 0;
}
