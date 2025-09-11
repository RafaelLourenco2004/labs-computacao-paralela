#include <stdio .h>
#include <pthread.h>
#include <stdlib.h>

#define VECTOR_SIZE 100000000
#define NUM_THREADS 4 

long long global_sum = 0;
long long v[VECTOR_SIZE];

typedef struct Chunk{
    int start;
    int end;
};

void *sum_chunk(void *arg){
    Chunk *chunk = (Chunk *) arg;
    for (int i = chunk->start; i <= chunk->end; i++){
        global_sum += v[i];
    }
    return NULL;
}

int main(){
    for (int i=0; i<VECTOR_SIZE; i++){
        v[i] = i+1;
    }

    int chunk_per_thread = VECTOR_SIZE / NUM_THREADS;
    int remaining = VECTOR_SIZE % NUM_THREADS;

    Chunk threads_chunks[NUM_THREADS];

    pthread_t threads[NUM_THREADS];
    for (int i=0; i<NUM_THREADS; i++){
        int start = i * chunk_per_thread;
        int end = start + chunk_per_thread - 1;
        if (i == NUM_THREADS -1){
            end += remaining;
        }
        threads_chunks[i].start = start;
        threads_chunks[i].end = end;
        int status = pthread_create(&threads[i], NULL, sum_chunk, &threads_chunks[i]);
        if (status == 0){
            printf("Algo deu errado.");
            exit(1);
        }
    }

    
    return 0;
}