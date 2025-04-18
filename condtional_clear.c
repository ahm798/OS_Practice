#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct node node;
node * createNode(int data);
void insert(node **head, int data);
void printList(node *head);
int listSize(node *head);
void clearList(node ** head);


struct node{
	int data;
	node * next;
};

typedef struct{
	int data;
	node ** head;
	pthread_mutex_t* lock;
	pthread_cond_t* cond;
}ThreadArgs;


void* threadfunction(void *arg){
	ThreadArgs *args = (ThreadArgs *)arg;
	pthread_mutex_lock(args -> lock);
	insert(args->head, args->data);
	if(listSize(*args->head) == 10){
		printList(*args->head);
		printf("Thread %ld: List size is 10, notifying...\n", pthread_self());
		pthread_cond_signal(args -> cond);
	}
	pthread_mutex_unlock(args -> lock);
	return NULL;
}

void * threadfunction2(void *arg){
	ThreadArgs *args = (ThreadArgs *)arg;
	pthread_mutex_lock(args -> lock);
	while (listSize(*args->head) !=10){
		pthread_cond_wait(args -> cond, args -> lock);		
	}
	printf("Thread %ld: List size is 10, clearing the List...\n", pthread_self());
	clearList(args->head);
	printf("Thread %ld: List cleared\n", pthread_self());
	pthread_mutex_unlock(args -> lock);
	pthread_exit(NULL);	
	return NULL;
}

int main(){
	node* head = NULL;
	pthread_t cls;
	
	pthread_t threads[10];
	ThreadArgs * dargs = (ThreadArgs*) malloc(10 * sizeof(ThreadArgs));

	pthread_mutex_t lock;
	pthread_mutex_init(&lock, NULL);
	pthread_cond_t cond;
	pthread_cond_init(&cond, NULL);

	ThreadArgs * dargs2 = (ThreadArgs*) malloc(sizeof(ThreadArgs));
	dargs2->head = &head;
	dargs2->lock = &lock;
	dargs2->cond = &cond;
	dargs2->data = 0;
	pthread_create(&cls, NULL, threadfunction2, dargs2);

	for(int i = 0; i < 10; i++){
		dargs[i].data = i+1; 
		dargs[i].head = &head;
		dargs[i].lock = &lock;
		dargs[i].cond = &cond;
		pthread_create(&threads[i], NULL, threadfunction, &dargs[i]);
	}
	for(int i = 0; i < 10; i++){
		pthread_join(threads[i], NULL);
	}


	printf("list length : %d\n", listSize(head));
	printList(head);
	return 0;
}


node* createNode(int data){
	node *newNode = (node*) malloc(sizeof(node));
	newNode -> data = data;
	newNode -> next = NULL;
	return newNode;
}

void insert(node **head, int data){
	node* newNode = createNode(data);
	newNode -> next = *head;
	*head = newNode;
	printf("Thread %ld: Inserted %d\n", pthread_self(), data);
}

void printList(node *head){
	node *current = head;
	while (current !=NULL){
		printf("%d -> ", current->data);
		current  = current ->next;
	}
	printf("END\n");
}

int  listSize(node *head){
	node * current  = head;
	int len = 0;
	while (current != NULL){
		len += 1;
		current = current -> next;
	}
	return len;
}


void clearList(node ** head){
    node * current = *head;
    node * nextNode;
    while(current != NULL){
        nextNode = current->next;
        free(current);
        current = nextNode;
    }
    *head = NULL;
}