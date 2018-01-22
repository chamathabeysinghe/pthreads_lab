//
// Created by chamath on 1/21/18.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <pthread.h>
#include "timer.h"

/**
 * For the linked list implementation
 */
struct Node{
    int value;
    struct Node* next;
};

/**
 * For selecting which action to execute by the thread
 */
struct Action {
    int value;
    char command;
};

typedef struct Node *node;
typedef struct Action *action;

int n = 1000;
int m =10000;
int mCurrent =0;
int mMember=5000,mInsert=3000,mDelete=2000;

int thread_count;
pthread_mutex_t linkedListMutex;
pthread_mutex_t actionGeneratorMutex;


/**
 * Support function to generate a node in linked list
 * @return
 */
node createNode(){
    node temp;
    temp = (node)malloc(sizeof(struct Node));
    temp->next = NULL;
    return temp;
}

/**
 * Support function to create a action
 * @param value
 * @param c
 * @return
 */
action createAction(int value,char c){
    action act;
    act = (action)malloc(sizeof(struct Action));
    act->value = value;
    act->command = c;
}

/**
 * Function to create set of random functions
 * @return
 */
action getNextAction(){
    pthread_mutex_lock(&actionGeneratorMutex);
    action instruction;

    if(mCurrent<m){
        int randomNumber= rand();
        if(mCurrent<mMember){
            instruction = createAction(randomNumber,'M');
        }
        else if(mCurrent<(mMember+mInsert)){
            instruction = createAction(randomNumber,'I');
        }
        else{
            instruction = createAction(randomNumber,'D');
        }
    }
    else {
        instruction = createAction(-1,'E');
    }
    mCurrent += 1;
    pthread_mutex_unlock(&actionGeneratorMutex);
    return instruction;

}
/*##################LINKED LIST IMPLEMENTATION########################### */

int Member(node head,int value){

    pthread_mutex_lock(&linkedListMutex);
    node currentNode = head;

    while(currentNode!=NULL){
        if(currentNode->value==value)
            return 1;
        currentNode = currentNode->next;
    }
    pthread_mutex_unlock(&linkedListMutex);
    return 0;
}

node Insert(node head,int value){
    pthread_mutex_lock(&linkedListMutex);
    node temp,p;
    temp = createNode();
    temp->value = value;
    if(head == NULL){
        head=temp;
    }
    else{
        p = head;
        while(p->next!=NULL){
            p = p->next;
        }
        p->next = temp;
    }
    pthread_mutex_unlock(&linkedListMutex);
    return head;
}

node Delete(node head,int value){
    pthread_mutex_lock(&linkedListMutex);
    if(head==NULL){
        return head;
    }
    if(head->value==value){
        node temp = head->next;
        free(head);
        *head = *temp;
        return head;
    }
    else{
        node p = head;
        while(p->next!=NULL){
            if(p->next->value == value){
                node temp = p->next->next;
                free(p->next);
                p->next = temp;
                return head;
            }
            p = p->next;
        }
    }

    pthread_mutex_unlock(&linkedListMutex);

    return head;
}

node initLinkedList(int size){
    srand(time(NULL));
    node head = Insert(NULL,rand());

    int i=0;
    for(i=0;i<size-1;i++){
        int randomNumber = rand();
        while(Member(head,randomNumber)){
            randomNumber = rand();
        }
        Insert(head,randomNumber);
    }
    return head;
}

/*##################END OF LINKED LIST IMPLEMENTATION########################### */

void execute(action next,node head){
    if(next->command=='M'){
        Member(head,next->value);
    }
    else if(next->command=='I'){
        Insert(head,next->value);
    }
    else if(next->command=='D'){
        Delete(head,next->value);
    }
}

void *manage(void* input ){
    node head = (node)input;
    action next = getNextAction();

    while(next->command!='E'){
        execute(next,head);
        next = getNextAction();
    }
}

void *Hello(void* rank){
    pthread_mutex_lock(&linkedListMutex);

    long my_rank = (long)rank;
    printf("My rank is %ld\n",my_rank);

    pthread_mutex_unlock(&linkedListMutex);
}

int main(int argc,char* argv[]){
    long thread;
    pthread_t *thread_handles;
    thread_count = strtol(argv[1],NULL,10);

    thread_handles = malloc(thread_count*sizeof(pthread_t));
    pthread_mutex_init(&linkedListMutex, NULL);
    pthread_mutex_init(&actionGeneratorMutex, NULL);
    double start,finish,elapsed;
    node head = initLinkedList(n);

    GET_TIME(start);

    for(thread = 0; thread<thread_count;thread++){
        pthread_create(&thread_handles[thread],NULL,manage,(void*) head);
    }
    for(thread=0;thread<thread_count;thread++){
        pthread_join(thread_handles[thread],NULL);
    }
    GET_TIME(finish);

    printf("%e",(finish-start));
}