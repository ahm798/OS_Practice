#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct {
    pthread_mutex_t counter_mutex;
    pthread_cond_t read_cond;
    pthread_cond_t write_cond;
    int resource_counter;
} resource_args;


void * reader(void *args){

    // reader Entry point 
    resource_args *res_args = (resource_args *)args;

    pthread_mutex_lock(&res_args->counter_mutex);
    while(res_args->resource_counter == -1){
        pthread_cond_wait(&res_args->read_cond, &res_args->counter_mutex);
    }
    res_args->resource_counter++;
    pthread_mutex_unlock(&res_args->counter_mutex);
    // reading resource file 
    printf("Reader %ld: Reading resource. Counter: %d\n", pthread_self(), res_args->resource_counter);

    // reader exit point 
    pthread_mutex_lock(&res_args->counter_mutex);
    res_args->resource_counter--;
    if(res_args->resource_counter == 0){
        pthread_cond_signal(&res_args->write_cond);
    }
    pthread_mutex_unlock(&res_args->counter_mutex);
    return NULL;
}

void * writer(void *args){

    // writer entry point
    resource_args *res_args = (resource_args *)args;
    pthread_mutex_lock(&res_args->counter_mutex);
    while(res_args -> resource_counter != 0){
        pthread_cond_wait(&res_args->write_cond, &res_args->counter_mutex);
    }
    res_args->resource_counter = -1;
    pthread_mutex_unlock(&res_args->counter_mutex);
    // writing resource file
    printf("Writer %ld: Writing resource. Counter: %d\n", pthread_self(), res_args->resource_counter);

    // writer exit point
    // finish writing and reset the resource counter
    //notify other reading threads to read thhe resource

    pthread_mutex_lock(&res_args->counter_mutex);
    res_args->resource_counter = 0;
    pthread_cond_broadcast(&res_args->read_cond);
    pthread_cond_signal(&res_args->write_cond);
    pthread_mutex_unlock(&res_args->counter_mutex);
    return NULL;
}

int main(){

    resource_args res_args;
    res_args.resource_counter = 0;
    pthread_mutex_init(&res_args.counter_mutex, NULL);
    pthread_cond_init(&res_args.read_cond, NULL);
    pthread_cond_init(&res_args.write_cond, NULL);
    // pthread_t readers[5];
    // pthread_t writers[2];
    // for(int i = 0; i < 5; i++){
    //     pthread_create(&readers[i], NULL, reader, &res_args);
    // }
    // for(int i = 0; i < 2; i++){
    //     pthread_create(&writers[i], NULL, writer, &res_args);
    // }
    // for(int i = 0; i < 5; i++){
    //     pthread_join(readers[i], NULL);
    // }
    // for(int i = 0; i < 2; i++){
    //     pthread_join(writers[i], NULL);
    // }
    pthread_t threads [10];
    for(int i = 0; i < 10; i++){
        if(i % 3 == 0){
            pthread_create(&threads[i], NULL, writer, &res_args);
        }else{
            pthread_create(&threads[i], NULL, reader, &res_args);
        }
    }
    for(int i = 0; i < 10; i++){
        pthread_join(threads[i], NULL);
    }
    return 0;
}
