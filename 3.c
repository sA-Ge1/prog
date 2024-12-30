#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int a[4][4], b[4][4];

void *matrixeval(void *val) {
    int thno = *(int *)val;
    for (int i = 0; i < 4; i++) {
        b[thno][i] = a[thno][i];
        for (int j = 0; j < thno; j++) {
            b[thno][i] *= a[thno][i];
        }
    }
    free(val);
    pthread_exit(NULL);
}

int main() {
    pthread_t tid[4];

    for (int i = 0; i < 4; i++) {
        printf("Enter the elements of row %d: ", i + 1);
        for (int j = 0; j < 4; j++) {
            scanf("%d", &a[i][j]);
        }
    }

    printf("Before processing:\n");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%d ", a[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < 4; i++) {
        int *arg = malloc(sizeof(int));
        *arg = i;
        pthread_create(&tid[i], NULL, matrixeval, arg);
    }

    for (int i = 0; i < 4; i++) {
        pthread_join(tid[i], NULL);
    }

    printf("After processing:\n");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%d ", b[i][j]);
        }
        printf("\n");
    }

    pthread_exit(NULL);
    return 0;
}
