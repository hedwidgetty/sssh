#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"alias.h"

struct node
{
	Alias* data;
	struct node* next;
};
typedef struct node Node;

void addFirst(Node* t, Node** head);
void addLast(Node* t, Node** head);
Node* createNode(Alias* data);
void addOrdered(Node* t, Node** head);
void deleteNode(char* alias, Node** head);
int compareAlias(char* one, char* alias);
void cleanNode(Node* n);
Node* findAlias(Node** head, char* alias);
void printList(Node* head);
void clearList(Node** head);
