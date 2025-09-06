#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define VECTOR_SIZE 9000000
#define NUM_PROCESSES 4

long long sum(int l, int r, const long long *v)
{
    long long total = 0;
    for (int i = l; i < r ;i++)
    {
        total += v[i];
    }
    return total;
}

int main()
{
    long long v[VECTOR_SIZE];
    for (int i=0; i < VECTOR_SIZE;i++){
        v[i] = i + 1; 
    }

    long long total_sum = 0;

    int fd[NUM_PROCESSES][2];
    for (int i=0;i<NUM_PROCESSES;i++){
        if (pipe(fd[i]) == -1){
            printf("Pipe call failed\n");
            exit(1);
        }
    }

    pid_t pid;
    for (int i=0;i<NUM_PROCESSES;i++){
        // if (pipe(buffer) == -1){
        //     printf("Pipe call failed\n");
        //     exit(1);
        // }

        pid = fork();

        if (pid < 0){
            printf("Fork failed\n");
            exit(1);
        }

        if (pid == 0){
            close(fd[i][0]);
            int chunk_size = VECTOR_SIZE / NUM_PROCESSES;
            int l = i * chunk_size;
            int r = i == (NUM_PROCESSES - 1) ? VECTOR_SIZE : (i+1) * chunk_size;
            long long chunk_sum = sum(l, r, v);
            write(fd[i][1], &chunk_sum, sizeof(long long));
            close(fd[i][1]);
            exit(0);
        }
       
        
    }
    
    
    long long partial_sum;
    for (int i=0;i<NUM_PROCESSES;i++){
        close(fd[i][1]);
        read(fd[i][0], &partial_sum, sizeof(long long));
        total_sum += partial_sum;
        close(fd[i][0]);
    }

    for (int i=0;i<NUM_PROCESSES;i++){
        wait(NULL);
    }

    printf("SUM = %lld\n", total_sum);
}