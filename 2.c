#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define PI 3.14159265358979323846

double taylor_series_sin(double x, int n) {
    double result = 0;

    for (int i = 0; i < n; i++) {
        double term = pow(-1, i) * pow(x, 2 * i + 1) / tgamma(2 * i + 2) / tgamma(i + 1);
        result += term;
    }

    return result;
}

int main() {
    int K, N, n;

    scanf("%d %d %d", &K, &N, &n);

    double y[K];

    for (int i = 0; i < K; i++) {
        pid_t pid = fork(); 

        if (pid < 0) {
            perror("Error creating process");
            return 1;
        }

        if (pid == 0) {
            double x = 2 * PI * i / N;
            double result = taylor_series_sin(x, n);
            printf("Process PID: %d, i = %d, sin(2*PI*i/N) = %lf\n", getpid(), i, result);
            exit(0);
        }
    }

    for (int i = 0; i < K; i++) {
        wait(NULL);
    }

    for (int i = 0; i < K; i++) {
        double x = 2 * PI * i / N;
        y[i] = taylor_series_sin(x, n);
    }

    FILE *file = fopen("results.txt", "w");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    for (int i = 0; i < K; i++) {
        fprintf(file, "y[%d] = %lf\n", i, y[i]);
    }

    fclose(file);

    return 0;
}
