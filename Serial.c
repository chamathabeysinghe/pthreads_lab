//
// Created by chamath on 1/21/18.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
struct Node{
    int value;
    struct Node* next;
};

typedef struct Node *node;

node createNode(){
    node temp;
    temp = (node)malloc(sizeof(struct Node));
    temp->next = NULL;
    return temp;
}

int Member(node head,int value){

    node currentNode = head;

    while(currentNode!=NULL){
        if(currentNode->value==value)
            return 1;
        currentNode = currentNode->next;
    }
    return 0;
}

node Insert(node head,int value){
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
    return head;
}

node Delete(node head,int value){
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
    return head;


}

int n = 0;
int m =0;
int mCurrent =0;
int mMember,mInsert,mDelete;

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

char* getNextAction(){

    char* command;
    if(mCurrent<m){
        int randomNumber= rand();
        if(mCurrent<mMember){
            command = "M"+randomNumber;
        }
        else if(mCurrent<(mMember+mInsert)){
            command = "I"+randomNumber;
        }
        else{
            command = "D"+randomNumber;
        }
    }
    else {
        command = "E";
    }

    mCurrent += 1;
    return command;
}

int main() {
    node head = initLinkedList(10);
    printf("Hello World\n");
//    node head = Insert(NULL,10);
    Insert(head,14);
    Insert(head,67);
    Insert(head,128);
    printf("%d\n",Member(head,10));
    printf("***********\n");
    Delete(head,67);
    printf("%d\n",Member(head,10));
    printf("%d\n",Member(head,14));
    printf("%d\n",head->value);
}
