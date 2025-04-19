#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t  m1= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t  m2= PTHREAD_MUTEX_INITIALIZER;

void * fn1(void *args){
	pthread_mutex_lock(&m1);
	printf("thread %ld grante m1 lock\n", pthread_self());
	sleep(1);
	pthread_mutex_lock(&m2);
	printf("thread %ld is grant m2\n", pthread_self());
	pthread_mutex_unlock(&m1);
	pthread_mutex_unlock(&m2);
	return NULL;
}

void * fn2(void* args){
	pthread_mutex_lock(&m2);
	printf("thread %ld is granted locaked m2\n", pthread_self());
	sleep(1);

	pthread_mutex_lock(&m1);
	printf("thread 2 is doing some shit \n");
	pthread_mutex_unlock(&m2);
	pthread_mutex_unlock(&m1);
	return NULL;
}

int main(){
	pthread_t t1, t2;
	
	pthread_create(&t1, NULL, fn1, NULL);
	pthread_create(&t2, NULL, fn2, NULL);
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	return 0;
}
