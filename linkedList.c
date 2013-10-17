#include "linkedList.h"




Node* createNode(Alias* d)
{
		
	Node* t= (Node*)calloc(1, sizeof(Node));
	t->data=d;
	return t;
}
	

void addFirst(Node* t, Node** head)
{
	t->next=*head;
	*head=t;
}

void addLast(Node* t, Node** head)
{
	Node* cur=*head;
	if(cur==NULL)
		addFirst(t, head);
	else
	{
		while(cur->next!=NULL)
			cur=cur->next;
		cur->next=t;
		t->next=NULL;
	}
}

void printList(Node* head)
{
	Node* cur=head;
	for(cur=head; cur!=NULL; cur=cur->next)
	{
		printf("alias %s=%s\n", (cur->data)->als, cur->data->cmd);		
	}	
	if(head==NULL)
		printf("Empty List!\n\n");
}
void addOrdered(Node* t, Node** head)
{
	Node* cur = *head;
	
	if(cur==NULL)
	{
		*head=t;
		
	}
	
	else if(strcmp((cur->data->als),(t->data->als))>0)
	{
		
		t->next=cur;
		*head=t;
		
	}
	else
	{
			
		while(cur->next!=NULL&&strcmp((cur->data->als),(t->data->als))<=0)
			cur=cur->next;
		
		t->next=cur->next;
		cur->next=t;
		
	}

}

int compareAlias(char* data, char* alias)
{
	return strcmp(data, alias);	
	
}

Node* findAlias(Node** head, char* alias)
{
	Node* cur=*head;
	
	while((cur!=NULL)&&(compareAlias((cur->data)->als, alias)!=0))
			cur=cur->next;
	if(cur==NULL)
	{	
		
		return NULL;
	}
	else
	{
		return cur;
	}
	
	
	


}

void deleteNode(char* alias, Node** head)
{
	Node* cur=*head;
	if(compareAlias(((*head)->data)->als, alias)!=0)
	{
		while((cur->next!=NULL)&&(compareAlias((cur->data)->als, alias)!=0))
			cur=cur->next;
		if(cur->next==NULL)
			printf("%s: not found\n", alias);
		else
		{
			
			Node* temp=cur->next;
			cur->next=cur->next->next;
			cleanNode(temp);
		}
	}
	else
	{
		*head=cur->next;
		cleanNode(cur);
	}	
	

}

void cleanNode(Node* n)
{
	free(n->data);
	free(n);

}

void clearList(Node** head)
{
	Node* cur= *head;
	
	while(cur!=NULL)
	{
		*head=cur->next;
		cleanNode(cur);
		cur=*head;
	}
	

}
