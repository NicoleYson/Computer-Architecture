/*
 ============================================================================
 Name        : fourth.c
 Author      : Nicole Yson
 Version     : 1.0
 Description : Hash Table
 ============================================================================
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>


/************* Hash Table Definistion ***************/

typedef size_t hash_size;

struct hashnode_s
{
	char *key;
	void *data;
	struct hashnode_s *next;
};

typedef struct hashtbl
{
	hash_size size;
	struct hashnode_s **nodes;
	hash_size (*hashfunc)(const char *);
} HASHTBL;

static char *mystrdup(const char *s)
{
	char *b;
	if(!(b=malloc(strlen(s)+1))) return NULL;
	strcpy(b, s);
	return b;
}

/***********   Hash Table Functions *************/

static hash_size defHashFunction(const char *key)
{
	hash_size hash=0;

	while(*key) hash+=(unsigned char)*key++;

	return hash;
}

HASHTBL *createHashTable(hash_size size, hash_size (*hashfunc)(const char *))
{
	HASHTBL *hashtbl;

	if(!(hashtbl=malloc(sizeof(HASHTBL)))) return NULL;

	if(!(hashtbl->nodes=calloc(size, sizeof(struct hashnode_s*)))) {
		free(hashtbl);
		return NULL;
}

	hashtbl->size=size;

	if(hashfunc) hashtbl->hashfunc=hashfunc;
	else hashtbl->hashfunc=defHashFunction;

	return hashtbl;
}

int insertNode(HASHTBL *hashtbl, const char *key, void *data)
{
	struct hashnode_s *node;
	hash_size hash=hashtbl->hashfunc(key)%hashtbl->size;


	node=hashtbl->nodes[hash];
	while(node) {
		if(!strcmp(node->key, key)) {
			node->data=data;
			return 0;
		}
		node=node->next;
	}

	if(!(node=malloc(sizeof(struct hashnode_s)))) return -1;
	if(!(node->key=mystrdup(key))) {
		free(node);
		return -1;
	}
	node->data=data;
	node->next=hashtbl->nodes[hash];
	hashtbl->nodes[hash]=node;

	return 0;
}


void *searchNode(HASHTBL *hashtbl, const char *key)
{
	struct hashnode_s *node;
	hash_size hash=hashtbl->hashfunc(key)%hashtbl->size;

/*	fprintf(stderr, "searchNode() key=%s, hash=%d\n", key, hash);*/

	node=hashtbl->nodes[hash];
	while(node) {
		if(!strcmp(node->key, key)) return node->data;
		node=node->next;
	}

	return NULL;
}


/**************  Miscellaneouse Functions *****************/

char *trimAllSpaces(char *str)
{
	  char *end;

	  /* Trim leading space */
	  while(isspace(*str)) str++;

	  if(*str == 0)
		return str;

	  /* Trim trailing space */
	  end = str + strlen(str) - 1;
	  while(end > str && isspace(*end)) end--;

	  /* Write new null terminator */
	  *(end+1) = 0;

	  return str;
}

int isNumeric (const char * s)
{
    if (s == NULL || *s == '\0' || isspace(*s))
      return 0;
    char * p;
    strtod (s, &p);
    return *p == '\0';
}


/************** !!!!! MAIN LOGIC !!!!! ******************/

int main ( int argc, char *argv[] )
{

	if ( argc != 2 )
	{
		printf( "error\n" );
	}
	else

	{
		FILE *file = fopen( argv[1], "r" );

		if ( file == 0 )
		{
			printf( "error\n" );
		}
		else
		{

			char line_buffer[BUFSIZ];
			/* Define delimiter */
			char* delim = "\t";			/* delimiter is tab */

			HASHTBL *hashtbl;
			hashtbl=createHashTable(1000, NULL);

            while(fgets(line_buffer, sizeof(line_buffer), file))
            {
            	char *command = trimAllSpaces (strtok (line_buffer, delim) );
            	char *Val =  trimAllSpaces ( strtok (NULL, delim) );

	            if ( !isNumeric(Val) )
            	{
            		printf ("error\n");
            	}
	            else
	            {

					if (strcmp(command, "i")  == 0)
					{
						if ( searchNode(hashtbl, Val) == NULL )
						{
							insertNode(hashtbl, Val, Val);
							printf("inserted\n");
						}
						else
						{
							printf("duplicate\n");
						}
					}
					else
					if (strcmp(command, "s")  == 0)
					{
							if ( searchNode(hashtbl, Val) == NULL )
							{
								printf("absent\n");
							}
							else
							{
								printf("present\n");
							}

					}
					else printf("error\n");
	            }
            }
			fclose(file);
		}
	}
	return 0;
}

