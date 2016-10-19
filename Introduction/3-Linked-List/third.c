/*
 ============================================================================
 Name        : third.c
 Author      : Nicole Yson
 Version     : 1.0
 Description : Linkedlist manipulation (create/insert/delete/list)
 ============================================================================
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

struct node
{
    int data;
    struct node* next;
};

struct node *head = NULL;
struct node *curr;
#include<stdbool.h>

int  initVal = -65535;
/* A utility function to create a new node */
struct node *newNode(int new_data)
{
    /* allocate node */
    struct node* new_node = (struct node*) malloc(sizeof(struct node));

    /* put in the data  */
    new_node->data  = new_data;
    new_node->next =  NULL;

    return new_node;
}

struct node* searchNode(int data, struct node **prev)
{
    struct node *ptr = head;
    struct node *tmp = NULL;
    bool found = false;

/*     printf("\n Searching the list for data [%d] \n",data); */

    while(ptr != NULL)
    {
        if(ptr->data == data)
        {
            found = true;
            break;
        }
        else
        {
            tmp = ptr;
            ptr = ptr->next;
        }
    }

    if(true == found)
    {
        if(prev)
            *prev = tmp;
        return ptr;
    }
    else
    {
        return NULL;
    }
}

struct node*  removeNode(struct node* curr, int x)
{
    struct node* next;
    if (curr == NULL)
	{ /* Found the tail */
        return NULL;
    }
	else if (curr->data == x)
	{
        next = curr->next;
        free(curr);
        return next;
    } else
	{ /* Just keep going */
        curr->next = removeNode(curr->next, x);
        return curr;
    }
}


/* Insert a new node in a list. Function expects a pointer to head_ref to
   be able to modify head of the input linked list (similar to push())*/
void sortedInsert(struct node** head_ref, struct node* new_node)
{
    /* Special case for the head end */
    if (*head_ref == NULL || (*head_ref)->data >= new_node->data)
    {
    	new_node->next = *head_ref;
        *head_ref = new_node;
    }
    else
    {
        if(searchNode(new_node->data, NULL) != NULL)
        {
        	return;
        }
        else
        {
/*            printf("Search [data = %d] failed, Adding\n",new_node->data); */
        }

        /* Locate the node before the point of insertion */
        curr = *head_ref;
        while (curr->next!=NULL && curr->next->data < new_node->data)
        {
            curr = curr->next;
        }
        new_node->next = curr->next;
        curr->next = new_node;
    }
}


/*  Function to print linked list */
void printList(struct node *head)
{
    struct node *temp = head;
    int firstLine = 0;
    while(temp != NULL)
    {
    	if (firstLine == 0)
    	{
    		printf("%d", temp->data);
    		firstLine = 1;
    	}
    	else
    	{
        printf("\t%d", temp->data);
    	}
        temp = temp->next;
    }
    printf("\n");
}


/*  !!!!! MAIN LOGIC !!!!!  */
int main ( int argc, char *argv[] )
{

	    if ( argc != 2 )
	    {
	        printf( "error" );
	    }
	    else

	    {
	        FILE *file = fopen( argv[1], "r" );
	        /* fopen returns 0, the NULL pointer, on failure */
	        if ( file == 0 )
	        {
	            printf( "error" );
	        }
	        else
	        {
		        sortedInsert(&head, newNode(initVal));

		        int val, fileStat; char command;

		        fileStat = fscanf(file, "%c%d\n", &command, &val);
		        if (fileStat == 1)
		        {
		        	printf("error");
		        	return 0;
		        }
		        while (fileStat != EOF)
	            {
	            	if (command == 'i')
	            	{
	            		sortedInsert(&head, newNode(val));
	            	}
	            	else
		            	if (command == 'd')
	            	{
	            		head = removeNode(head, val);
	            	}
	            	else
	            	{
	            		printf("error");
	            		return 0;
	            	}
			        fileStat = fscanf(file, "%c%d\n", &command, &val);
			        if (fileStat == 1)
			        {
			        	printf("error");
			        	return 0;
			        }
	            }
	            fclose(file);
	            head = removeNode(head, initVal);

	            printList(head);
	        }
	    }

    return 0;
}



