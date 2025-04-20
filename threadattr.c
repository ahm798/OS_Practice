#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

void * foo(void *arg) {
    printf("Thread %ld: Hello, World!\n", pthread_self());
    return NULL;
}

int main(){
    pthread_t thread;
    pthread_attr_t attr;

    printf("Main thread %ld: Starting execution.\n", pthread_self());

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
    pthread_attr_setstacksize(&attr, 1024 * 1024); // Set stack size to 1MB
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);

    pthread_create(&thread, &attr, foo, NULL);
    pthread_attr_destroy(&attr); // Clean up the thread attributes
    printf("main %ld: Finished execution.\n", pthread_self());
    pthread_exit(NULL); // Exit the main thread
    return 0;
}