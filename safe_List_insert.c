#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct node node;
node * createNode(int data);
void insert(node **head, int data);
void printList(node *head);
int listSize(node *head);


struct node{
	int data;
	node * next;
};

typedef struct{
	int data;
	node ** head;
	pthread_mutex_t* lock;
}ThreadArgs;


void* threadfunction(void *arg){
	ThreadArgs *args = (ThreadArgs *)arg;
	pthread_mutex_lock(args -> lock);
	insert(args->head, args->data);
	pthread_mutex_unlock(args -> lock);
	return NULL;
}

int main(){
	node* head = NULL;
	
	pthread_t threads[3];
	ThreadArgs * dargs = (ThreadArgs*) malloc(3 * sizeof(ThreadArgs));

	pthread_mutex_t lock;
	pthread_mutex_init(&lock, NULL);

	for(int i = 0; i < 3; i++){
		dargs[i].data = i+1;
		dargs[i].head = &head;
		dargs[i].lock = &lock;
		pthread_create(&threads[i], NULL, threadfunction, &dargs[i]);
	}
	for(int i = 0; i < 3; i++){
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
