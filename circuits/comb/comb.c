
/*
 ============================================================================
 Name        : comb.c
 Author      : NY
 Version     :
 Copyright   :
 Description : CIRCUIT SIMULATION
 ============================================================================
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef struct LinkedListNode
{
	char name;
	int  data;	/* 0 - OFF, 1 - ON, other - not set */
	struct LinkedListNode *next;
} Node;

#define generateGrayCode(GC) (GC ^ (GC >> 1))
char OFF = '0', ON = '1';

/* LinkedList node manipulation functions */
Node *createNode(char name, int Flag);
Node *createTempNode(char name, int Flag, Node *next);
Node *addNodeFront(Node *head, char name, int Flag);
Node *addNode(Node *head, char name, int Flag);
void  initNode(Node *head);
Node *freeNode(Node *head);
Node *processAND (FILE* Circuits, Node *head);
Node *processOR (FILE* Circuits, Node *head);
Node *processNOT (FILE* Circuits, Node *head);
Node *processMULTIPLEXER(FILE* Circuits, Node *head);
Node *processDECODER(FILE* Circuits, Node *head);
Node *evaluateAND (Node *head, char operand1, char operand2, char outVAR);
Node *evaluateOR (Node *head, char operand1, char operand2, char outVAR);
Node *evaluateNOT (Node *head, char operand1, char outVAR);
Node *processOUTPUTVAR (FILE *Circuits, Node *head);
Node *processINPUTVAR (FILE *Circuits, FILE *Inputs, Node *head);

/* MISCELLANEOUS functions */
int  functionAND(Node *head, char operand1, char operand2);
int  functionNOT (Node *head, char operand1);
int  searchGrayCode(int grayCode);
int  binaryBits(int* selectors, int numSelectors);
int  *singleBit(int grayCode, int decoderSize);
int  searchNode(Node *head, char name);
// void initVARIABLES (FILE *Circuits, FILE *Inputs, Node *head);


/* TROUBLESHOOTING FUNCTIONS ONLY*/
void printNode(Node *head);

FILE* Circuits; FILE* Inputs;
int getCircuits = 0, getInputs = 0, idx1 = 0,  data, numVars, numOutputs, numInputs;
char Oper[15], iVAR, oVAR, *inputs, *outputs;
Node *head = NULL;

int main(int argc, char** argv)
{

	if (argc < 3)
	{
		printf("error\n");
		return 0;
	}
	Circuits = fopen(argv[1], "r");
	Inputs = fopen(argv[2], "r");
	if(Inputs == NULL  || Circuits == NULL)
	{
		printf("error\n");
		return 0;
	}
	while(getInputs != EOF)
	{
		while(getCircuits != EOF)
		{
			getCircuits = fscanf(Circuits, "%s\n",Oper);

			if (getCircuits == EOF)
			{
				break;
			}

			if (strcmp(Oper,"INPUTVAR") == 0)
			{
				head = processINPUTVAR (Circuits, Inputs, head);
			}
			else if(strcmp(Oper, "OUTPUTVAR") == 0)
			{
				head = processOUTPUTVAR (Circuits, head);
			}
			else if(strcmp(Oper, "AND") == 0)
			{
				head = processAND(Circuits, head);
			}
			else if(strcmp(Oper, "NOT") == 0)
			{
				head = processNOT(Circuits, head);
			}
			else if(strcmp(Oper, "OR") == 0)
			{
				head = processOR(Circuits, head);
			}
			else if(strcmp(Oper, "MULTIPLEXER") == 0)
			{
				head = processMULTIPLEXER(Circuits, head);
			}
			else if(strcmp(Oper, "DECODER") == 0)
			{
				head = processDECODER(Circuits, head);
			}
		}

		if(getInputs != EOF)
		{
			if(numOutputs == 1)
			{
				printf("%i\n", searchNode(head, outputs[0]));
			}
			else
			{
				idx1=0;
				while(idx1 < numOutputs-1)
				{
					printf("%i ", searchNode(head, outputs[idx1]));
					idx1++;
				}
				if(numOutputs > 1) printf("%i\n", searchNode(head, outputs[numOutputs-1]));
			}
		}

		/* initVARIABLES (Circuits, Inputs, head) */
		numOutputs = 0;
		numInputs = 0;
		getCircuits = 0;
		initNode(head);
		free(outputs);
		free(inputs);
		head = NULL;
		rewind(Circuits);
	}
	fclose(Circuits);
	fclose(Inputs);
	return 0;
}

/*FUNCTIONS */

Node *evaluateAND (Node *tNode, char operandA, char operandB, char outVAR)
{
	int operand1, operand2, result = 0;

	switch (operandA)
	{
		case '0'	: 	operand1 = 0; break;
		case '1' 	: 	operand1 = 1; break;
		default		: 	operand1 = searchNode(tNode, operandA);
	}
	switch (operandB)
	{
		case '0'	: 	operand2 = 0; break;
		case '1' 	: 	operand2 = 1; break;
		default		: 	operand2 = searchNode(tNode, operandB);
	}

	if (operand1 == 1 && operand2 == 1) result = 1;

	tNode =  addNode(tNode, outVAR, result);
	return tNode;
}

Node *evaluateOR (Node *tNode, char operandA, char operandB, char outVAR)
{
	int operand1, operand2, result = 1;

	switch (operandA)
	{
		case '0'	: 	operand1 = 0; break;
		case '1' 	: 	operand1 = 1; break;
		default		: 	operand1 = searchNode(tNode, operandA);
	}
	switch (operandB)
	{
		case '0'	: 	operand2 = 0; break;
		case '1' 	: 	operand2 = 1; break;
		default		: 	operand2 = searchNode(tNode, operandB);
	}

	if (operand1 == 0 && operand2 == 0) result = 0;

	tNode =  addNode(tNode, outVAR, result);
	return tNode;
}

Node *evaluateNOT (Node *tNode, char operandA, char outVAR)
{
	int result = 0;

	switch (operandA)
	{
		case '0'	: 	result = 1; break;
		case '1' 	: 	result = 0; break;
		default		: 	if (searchNode(tNode, operandA) == 0) result = 1;
	}
	tNode=addNode(tNode,outVAR,result);
	return tNode;
}

int functionAND(Node *tNode, char operandA, char operandB)
{
	int operand1, operand2;
	if(operandA == 0)
		operand1 = 0;
	else if (operandA == 1)
		operand1 = 1;
	else if(operandA == OFF)
		operand1 = 0;
	else if(operandA == ON)
		operand1 = 1;
	else
		operand1 = searchNode(tNode, operandA);

	if(operandB == 0)
		operand2 = 0;
	else if(operandB == 1)
		operand2 = 1;
	else if(operandB == OFF)
		operand2 = 0;
	else if(operandB == ON)
		operand2 = 1;
	else
		operand2 = searchNode(tNode, operandB);
	if(operand1 == 1 && operand2 == 1)
		return 1;
	return 0;
}

int functionNOT (Node *tNode, char operandA)
{
	int operand1;
	if(operandA == 0)
		return 1;
	else if( operandA == 1)
		return 0;
	else
	if(operandA == OFF)
		return 1;
	else if(operandA == ON)
		return 0;

	operand1 = searchNode(tNode, operandA);
	if(operand1 == 0)
		return 1;


	return 0;
}


Node *processOUTPUTVAR (FILE *Circuits, Node *head)
{
	getCircuits = fscanf(Circuits, "%i", &numVars);
	outputs = (char*) malloc(sizeof(char)*numVars);
	numOutputs = numVars;
	idx1 = 0;
	while(idx1 < numVars)
	{
		fscanf(Circuits, " ");
		getCircuits = fscanf(Circuits, "%c", &oVAR);
		outputs[idx1] = oVAR;
		head = addNode(head, oVAR, -1);
		idx1++;
	}
	return head;
}

Node* processINPUTVAR (FILE *Circuits, FILE *Inputs, Node *head)
{
	getCircuits = fscanf(Circuits, "%i", &numInputs);
	inputs = (char*) malloc(sizeof(char)*numInputs);
	idx1 = 0;
	while(idx1 < numInputs)
	{
		fscanf(Circuits, " ");
		getCircuits = fscanf(Circuits, "%c", &iVAR);
		getInputs = fscanf(Inputs, "%i", &data);
		head = addNode(head, iVAR, data);
		inputs[idx1] = iVAR;
		idx1++;
	}
	return head;
}

Node *processAND (FILE* Circuits, Node *tNode)
{
	char operandA, operandB, result;
	fscanf(Circuits, "%c %c %c", &operandA, &operandB, &result);

	tNode = evaluateAND(tNode, operandA, operandB, result);
	return tNode;
}

Node *processOR (FILE* Circuits, Node *tNode)
{
	char operandA, operandB, result;
	fscanf(Circuits, "%c %c %c", &operandA, &operandB, &result);
	return evaluateOR(tNode, operandA, operandB, result);
}

Node *processNOT (FILE* Circuits, Node *tNode)
{
	char operandA, outVAR;
	fscanf(Circuits, "%c %c", &operandA, &outVAR);
	tNode= evaluateNOT(tNode, operandA, outVAR);
	return tNode;
}

Node *processMULTIPLEXER(FILE* Circuits, Node *tNode)
{
	int idx1, numVARS, numSelectors, *outs, *selectors, grayCode, index;
	char iVAR, muxOutput;

	numVARS = 0; numSelectors = 0;

	fscanf(Circuits, "%i",&numVARS);

	idx1 = numVARS;

	while(idx1 >>= 1) ++numSelectors;

	outs = (int*) malloc(sizeof(int) * numVARS);
	selectors=(int*) malloc(sizeof(int)*numSelectors);

	for(idx1 = 0; idx1 < numVARS; idx1++)
	{
		fscanf(Circuits, " ");
		fscanf(Circuits, "%c", &iVAR);
		outs[idx1] = iVAR;
	}
	for(idx1 = 0; idx1 < numSelectors; idx1++)
	{
		fscanf(Circuits, " ");
		fscanf(Circuits, "%c", &iVAR);
		if(iVAR == OFF)
			selectors[idx1] = 0;
		else if(iVAR == ON)
			selectors[idx1] = 1;
		else
		{
			selectors[idx1] = searchNode(tNode, iVAR);
		}
	}
	fscanf(Circuits, " ");
	fscanf(Circuits, "%c", &muxOutput);

	grayCode = binaryBits(selectors, numSelectors);
	index = searchGrayCode(grayCode);

	if(outs[index] == OFF)
		return addNode(tNode, muxOutput, 0);
	else if(outs[index] == ON)
		return addNode(tNode, muxOutput, 1);

	tNode = addNode(tNode, muxOutput, searchNode(tNode, outs[index]));
	return tNode;
}

Node *processDECODER(FILE* Circuits, Node *tNode)
{
	int grayCode, temp, decoderSize, numCodes, idx1, idx2, *bitField;
	char *inputs, iVAR;

	decoderSize = 0;

	fscanf(Circuits, "%i",&decoderSize);
	numCodes = 1 << decoderSize;

	inputs = (char*) (malloc(sizeof(char) * decoderSize));
	for(idx1 = 0; idx1 < decoderSize; idx1++)
	{
		fscanf(Circuits, " ");
		fscanf(Circuits, "%c", &iVAR);
		inputs[idx1] = iVAR;
	}

	for(idx1 = 0; idx1 < numCodes; idx1++)
	{
		grayCode = generateGrayCode(idx1);
		bitField = singleBit(grayCode, decoderSize);
		fscanf(Circuits, " ");
		fscanf(Circuits, "%c", &iVAR);

		temp = 1;
		for(idx2 = 0; idx2 < decoderSize; idx2++)
		{
			if(bitField[idx2] == 0)
				temp = functionAND(tNode, temp, functionNOT(tNode, inputs[idx2]));
			else
				temp = functionAND(tNode, temp, inputs[idx2]);
		}

		tNode=addNode(tNode, iVAR, temp);
	}

	return tNode;
}

int searchGrayCode(int grayCode)
{
	int idx1;
	idx1 = grayCode;
	while( grayCode >>= 1)	idx1 ^= grayCode;
	return idx1;
}

int* singleBit(int grayCode, int decoderSize)
{
	int* result, idx1;
	result = (int*) calloc(sizeof(int), decoderSize);
	for(idx1 = 0; idx1 < decoderSize; idx1++)
	{
		result[decoderSize - idx1 - 1] = ((grayCode >> idx1) & 1);
	}
	return result;
}

int binaryBits(int* selectors, int numSelectors)
{
	int result, idx1;
	result = 0;
	idx1=0;
	while(idx1 < numSelectors)
	{
		result |= (selectors[numSelectors - idx1 - 1] << idx1);
		idx1++;
	}
	return result;
}

Node *createNode(char name, int Flag)
{
	Node *ptr;
	ptr=(Node*) malloc(sizeof(Node));
	ptr->name = name;
	ptr->data = Flag;
	ptr->next=NULL;
	return ptr;
}

Node *createTempNode(char name, int Flag, Node *next)
{
	Node *ptr;
	ptr=(Node*) malloc(sizeof(Node));
	ptr->name = name;
	ptr->data = Flag;
	ptr->next=next;
	return ptr;
}

Node *addNodeFront(Node *head, char name, int Flag)
{
	Node *n=createTempNode(name, Flag, head);
	n->next=head;
	return n;
}

Node *addNode(Node *head, char name, int Flag)
{
	Node *ptr=head;
	Node *toAdd;
	if(ptr == NULL)
		return createNode(name, Flag);
	if(ptr->name == name)
	{
		ptr->data=Flag;
		return head;
	}
	while(ptr->next != NULL)
	{
		if(ptr->next->name == name)
		{
			ptr->next->data=Flag;
			return head;
		}
		ptr = ptr->next;
	}
	toAdd=createNode(name, Flag);
	ptr->next=toAdd;
	return head;
}

Node *freeNode(Node *tNode)
{
	Node *next;
	next=tNode->next;
	free(tNode);
	return next;
}

void initNode(Node *head)
{
	while(head != NULL) head=freeNode(head);
	return;
}

int searchNode(Node *head, char name)
{
	Node *ptr;
	for(ptr=head; ptr!=NULL; ptr=ptr->next) if(ptr->name == name) return ptr->data;
	return 9;
}


void initVARIABLES (FILE *Circuits, FILE *Inputs, Node *head)
{
	numOutputs = 0;
	numInputs = 0;
	getCircuits = 0;
	initNode(head);
	free(outputs);
	free(inputs);
	head = NULL;
	rewind(Circuits);
}


/* TROUBLESHOOTING */
void printNode(Node *head){
	Node *ptr;
	if(head==NULL)
		printf("NOTHING!!!\n");
	for(ptr=head; ptr!=NULL; ptr=ptr->next)
	{
		if(ptr->next==NULL) printf("%c: %i\n",ptr->name,ptr->data);
		else printf("%c: %i ",ptr->name, ptr->data);
	}
}

