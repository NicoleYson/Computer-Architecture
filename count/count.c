
/*
 ============================================================================
 Name        : count.c
 Author      : Nicole Yson
 Version     : 1.0
 Description : This assignment requires you to use hash table to count number
               of unique addresses in the trace of the memory addresses
               performed by a program. You need to implement a program called
               count. The input of count program is a text file with 64-bit
               addresses and the output is the number of unique addresses in
               the trace.
 ============================================================================
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

typedef int HASHSIZE_TYPE;									/* just in case hash size type will be change in the future */
typedef unsigned long long int INPUT_TYPE;							/* define type of input */
															/* Spec is 64bit hexa value (long long) */

#define HASHMAP_SIZE 1000									/* define size of hash map here */

/***************** Linked List Structure ********************/
typedef struct LinkedListNode
{
	INPUT_TYPE data;
	struct LinkedListNode *next;
} LLNODE;

/***************** Hash Table Definition ********************/
typedef struct hashtbl
{
	HASHSIZE_TYPE size;
	struct LinkedListNode **nodes;
	HASHSIZE_TYPE (*hashfunc)(const INPUT_TYPE);
} HASHTBL;

/********************** Hash Function ***********************/
/*
 * Remainder of "hexa value" / "hash table size"
 *
 */

static HASHSIZE_TYPE HashFunction( INPUT_TYPE data)
{
	HASHSIZE_TYPE hash = data % HASHMAP_SIZE;
	return hash;
}

/*********************** Create Table ***********************/
HASHTBL *createHashTable(HASHSIZE_TYPE size, HASHSIZE_TYPE (*hashfunc)(const INPUT_TYPE))
{
	HASHTBL *hashtbl;
	if(!(hashtbl = malloc(sizeof(HASHTBL)))) return NULL;
	if(!(hashtbl->nodes = calloc(size, sizeof(LLNODE))))	/* zero-initialize HASH TABLE */
	{
		free(hashtbl);										/* free allocated mem block for hashtbl */
		return NULL;
	}
	hashtbl->size = size;									/* set size of hash table */
	if(hashfunc) hashtbl->hashfunc = hashfunc;
	else hashtbl->hashfunc = HashFunction;
	return hashtbl;
}

/************* Insert Node Into Linked List  ***************/
int insertNode(HASHTBL *hashtbl, INPUT_TYPE data)
{
	HASHSIZE_TYPE row = HashFunction(data);					/* compute hash (row) for passed hexa */
	LLNODE *temp;											/* create temporary linked list name temp */

	temp = hashtbl->nodes[row];
/*
	while(temp != NULL)
	{
		temp = temp->next;
	}
*/
	temp = malloc(sizeof(LLNODE));							/* allocate mem block to temp */
	temp->data = data;										/* linked list temp FIRST node */
	temp->next = hashtbl->nodes[row];						/* append the old linked list to temporary list */
	hashtbl->nodes[row] = temp;								/* replace the current row with temporary list */

	return 0;
}

/* Searches for passed hexa in the hash chained table. then call insertNode if NOT found
 * Return 0 if found, 1 if not found.  Value will be used to increment uniqueCtr			*/
int uniqueNode(HASHTBL *hashtbl, INPUT_TYPE data)
{
	LLNODE *node;											/* linked list structure */
	HASHSIZE_TYPE row = HashFunction(data);					/* compute hash (row) for passed hexa */
	node = hashtbl->nodes[row];								/* pointer to the hash table row's first node of the linked list */
	while(node != NULL)
	{														/* traverse through linked list */
		if (node->data == data) return 0;					/* compare hexa to linked list data, return 0 if found */
		node = node->next;
	}
	insertNode(hashtbl, data);								/* not found, so insert the hexa */
	return 1;												/* return value will be use to increment uniqueCtr */
}

/***************  TROUBLESHOOTING Functions ****************/
/*
void printHashMapChained(HASHTBL *hashtbl, int size)
{
    int i;
    for(i = 0;i<size;i++)
    {
        printf("hashMapChained[%d]:\t",i);
    	LLNODE *node;
		node = hashtbl->nodes[i];
        while(node != NULL)
        {
            printf("%lX ", node->data );
            node = node->next;
        }
        printf("\n");
    }
}
*/

/************** !!!!! MAIN LOGIC !!!!! ******************/

int main ( int argc, char *argv[] )
{
	/*  VARIABLES DECLARATION AREA  */
	int fileStat, uniqueCtr;
	INPUT_TYPE hexa;
	HASHTBL *hashtbl;
	FILE *file;
	/*------------------------------*/

	if ( argc != 2 )										/* error out if no argument passed */
	{
		printf( "error" );
		return 0;
	}

	file = fopen( argv[1], "r" );
	if ( file == 0 )										/* error out if unable to open file for some reasons */
	{
		printf( "error" );
		return 0;
	}

	hashtbl = createHashTable(HASHMAP_SIZE, NULL);			/* First thing, create Hashtable */

	uniqueCtr = 0;											/* initialize counter */
	fileStat = fscanf(file, "%llX", &hexa);				/* get first line */
	while (fileStat != EOF && fileStat != 0 )
	{
		/*
		 *  Insert HERE whatever validation is required to filter correct HEXA input
		*/
		if (uniqueNode(hashtbl, hexa)) uniqueCtr++;			/* add 1 to uniqueCtr if hexa is unique  */
		fileStat = fscanf(file, "%llX", &hexa);
	}

	printf ("%d", uniqueCtr);								/* this is it! */
	fclose(file);

/*	 printHashMapChained(hashtbl, HASHMAP_SIZE); */
	return 0;
}


