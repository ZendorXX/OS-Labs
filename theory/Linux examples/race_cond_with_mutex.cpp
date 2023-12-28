// how to compile gcc -pthread race_cond_with_mutex.cpp

#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mutex;
int sum = 0;

void* increment_function_plus_200(void *args) {
    for (int i=0; i<100; i++)
    {
        pthread_mutex_lock(&mutex);
        sum = sum + 2;
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(0);
}

int main() {
    const int threadCount = 1000;
    pthread_mutex_init(&mutex, NULL);
    pthread_t tid[threadCount];
    for (int i=0; i<threadCount; i++)
    {
        pthread_create(&tid[i], NULL, increment_function_plus_200, NULL);
    }
    for (int i=0; i<threadCount; i++)
    {
        pthread_join(tid[i], NULL);
    }
    printf("sum = %d\n",sum);
    return 0;
}