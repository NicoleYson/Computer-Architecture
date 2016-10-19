/*
 ============================================================================
 Name        : seq.c
 Author      : NY
 Version     :
 Copyright   : 
 Description : Hello World in C, Ansi-style
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

#define generateGrayCode(x) (x ^ (x >> 1))
char OFF = '0', ON = '1';

Node *createNode(char name, int Flag);
Node *createTempNode(char name, int Flag, Node *next);
Node *addNodeFront(Node *head, char name, int Flag);
Node *addNode(Node *head, char name, int Flag);
int  searchNode(Node *head, char name);
Node *processOUTPUTVAR (FILE *Circuits, Node *tNode);
Node *processINPUTVAR (FILE *Circuits, FILE *Inputs, Node *head);
void processCLOCK (FILE *Circuits, Node *tNode);
void processDFLIPFLOP (FILE *Circuits, Node *tNode);
Node *processAND (FILE *Circuits, Node *tNode);
Node *processOR (FILE *Circuits, Node *tNode);
Node *processNOT (FILE *Circuits, Node *tNode);
Node *processMULTIPLEXER(FILE *Circuits, Node *tNode);
Node *processDECODER(FILE *Circuits, Node *tNode);
Node *evaluateAND (Node *tNode, char Operand1, char Operand2, char outVAR);
Node *evaluateOR (Node *tNode, char Operand1, char Operand2, char outVAR);
Node *evaluateNOT (Node *tNode, char Operand1, char outVAR);
int  functionNOT (Node *tNode, char Operand1);
int  functionAND(Node *tNode, char Operand1, char Operand2);
int  searchGrayCode(int x);
int  binaryBits(int *selectors, int numSelectors);
int  *singleBit(int grayCode, int numInputs);
void printOutputs(char **LLrow, int numOutputs);
void printList(Node *head);			/* TROUBLESHOOTING */

FILE *Circuits, *Inputs;
char Oper[15], iVAR, oVAR, clock = '0', **outputs = NULL, **inputs = NULL, *circuitFile, *inputsFile;
int  numInputs, numOutputs, data, inputCtr, grayIndex, idx1 = 0, idx2, getCircuit = 0, maxLoop = 99, mainLoop = 0;
int  getInput = 0;
Node *head = NULL;

int main(int argc, char **argv)
{
	if (argc < 3)
	{
		printf("error\n");
		return 0;
	}

	Circuits = fopen(argv[1], "r");
	Inputs = fopen(argv[2], "r");

	if (Inputs == NULL || Circuits == NULL)
	{
		printf("error\n");
		return 0;
	}

	while (mainLoop < maxLoop)
	{
		while (getCircuit != EOF)
		{
			getCircuit = fscanf(Circuits, "%s\n", Oper);
			if (getCircuit == EOF) break;
			if (strcmp(Oper,"INPUTVAR") == 0)
			{
				head = processINPUTVAR(Circuits, Inputs, head);
			}
			else if (strcmp(Oper, "OUTPUTVAR") == 0)
			{
				head = processOUTPUTVAR (Circuits, head);
			}
			else if (strcmp(Oper, "CLOCK") == 0)
			{
				processCLOCK (Circuits, head);
			}
			else if (strcmp(Oper, "DFLIPFLOP") == 0)
			{
				processDFLIPFLOP (Circuits, head);
			}
			else if (strcmp(Oper, "NOT") == 0)
			{
				head = processNOT(Circuits, head);
			}
			else if (strcmp(Oper, "AND") == 0)
			{
				head = processAND(Circuits, head);
			}
			else if (strcmp(Oper, "OR") == 0)
			{
				head = processOR(Circuits, head);
			}
			else if (strcmp(Oper, "MULTIPLEXER") == 0)
			{
				head = processMULTIPLEXER(Circuits, head);
			}
			else if (strcmp(Oper, "DECODER") == 0)
			{
				head = processDECODER(Circuits, head);
			}
		}
		rewind(Circuits);
		getCircuit = 0;
		mainLoop++;
	}
	idx1 = 0;

	printOutputs(inputs, numInputs);
	printOutputs(outputs, numOutputs);
	return 0;
}

/*  FUNCTIONS  */

void printOutputs(char **LLrow, int numOutputs)
{
	int idx1, idx2, nLoop;
	for (idx1 = 0; idx1 < numOutputs; idx1++)
	{
		nLoop = LLrow[idx1][1] + 1;
		printf("%c: ", LLrow[idx1][0]);
		for (idx2 = 2; idx2 < nLoop; idx2++) printf("%i ", LLrow[idx1][idx2]);
		printf("%i\n", LLrow[idx1][nLoop]);
	}
}

Node *processOUTPUTVAR (FILE *Circuits, Node *head)
{
	getCircuit = fscanf(Circuits, "%i", &numOutputs);
	if (outputs == NULL)
	{
		outputs = (char**) malloc(sizeof(char*) * numOutputs);
		for (idx1 = 0; idx1 < numOutputs; idx1++)
		{
			getCircuit = fscanf(Circuits, " ");


			getCircuit = fscanf(Circuits, "%c", &oVAR);

			outputs[idx1] = (char*) malloc(sizeof(char)*maxLoop);
			outputs[idx1][0] = oVAR;
			outputs[idx1][1] = maxLoop;
			for (idx2=2; idx2<= maxLoop; idx2++) outputs[idx1][idx2] = -1;
			head = addNode(head, oVAR, -1);
		}
	}
	return head;
}

Node *processINPUTVAR (FILE *Circuits, FILE *Inputs, Node *head)
{
	getCircuit = fscanf(Circuits, "%i", &numInputs);
	if (inputs == NULL)
	{
		inputs = (char**) malloc(sizeof(char*)*numInputs);
		for (idx1 = 0; idx1 < numInputs; idx1++)
		{
			getInput = fscanf(Inputs, "%c: ", &iVAR);
			getInput = fscanf(Inputs, "%i", &inputCtr);
			if (inputCtr > maxLoop) maxLoop = inputCtr;
			inputs[idx1] = (char*) malloc(sizeof(char)*inputCtr + 2);
			inputs[idx1][0] = iVAR;
			inputs[idx1][1] = inputCtr;
			for (idx2 = 2; idx2 <=inputCtr+1; idx2++)
			{
				getInput = fscanf(Inputs, "%i", &data);
				inputs[idx1][idx2] = data;
			}
			getInput = fscanf(Inputs, " ");
		}
	}
	for (idx1 = 0; idx1 <numInputs;idx1++)
	{
		idx2 = 0;
		getCircuit = fscanf(Circuits, " ");
		getCircuit = fscanf(Circuits, "%c", &iVAR);
		while (inputs[idx2][0] != iVAR) idx2++;
		grayIndex = mainLoop % (inputs[idx2][1]);
		head = addNode(head, inputs[idx2][0], inputs[idx2][2+grayIndex]);
	}
	return head;
}

void processCLOCK (FILE *Circuits, Node *tNode)
{
	getCircuit = fscanf(Circuits, " ");
	getCircuit = fscanf(Circuits, "%c", &clock);
	if (inputs != NULL)
	{
		idx2 = 0;
		while (inputs[idx2][0] != clock)
		{
			idx2++;
		}
		maxLoop = inputs[idx2][1];
		if (outputs != NULL)
		{
			for (idx1 = 0; idx1 < numOutputs; idx1++) if (outputs[idx1][1] >= maxLoop) outputs[idx1][1] = maxLoop;
		}

	}
}

void processDFLIPFLOP (FILE *Circuits, Node *tNode)
{
	char input, clock, output;
	int value0, indexOutput, result, inputCtr;

	getCircuit = fscanf(Circuits, "%i %c %c %c", &value0, &input, &clock, &output);

	indexOutput=0;
	while (outputs[indexOutput][0] != output && indexOutput < numOutputs)  indexOutput++;
	result = searchNode(head, output);
	if (searchNode(head, clock) == 0)
	{
		if (mainLoop == 0) result = value0;
	}
	else if (searchNode(head, clock) == 1)
	{
		if (input == OFF || searchNode(head, input) == 0)
			result = 0;
		else if (input == ON || searchNode(head, input) == 1)
			result = 1;
	}
	addNode(head, output, result);
	inputCtr = outputs[indexOutput][1];
	outputs[indexOutput][mainLoop+2 % inputCtr] = result;
}

Node *processAND (FILE *Circuits, Node *tNode)
{
	char operand1, operand2, outVAR;
	fscanf(Circuits, "%c %c %c", &operand1, &operand2, &outVAR);
	tNode = evaluateAND(tNode, operand1, operand2, outVAR);
	return tNode;
}

Node *processOR (FILE *Circuits, Node *tNode)
{
	char operand1, operand2, outVAR;
	fscanf(Circuits, "%c %c %c", &operand1, &operand2, &outVAR);
	tNode =  evaluateOR(tNode, operand1, operand2, outVAR);
	return tNode;
}

Node *processNOT (FILE *Circuits, Node *tNode)
{
	char operand1, outVAR;
	fscanf(Circuits, "%c %c", &operand1, &outVAR);
	tNode= evaluateNOT(tNode, operand1, outVAR);
	return tNode;
}

Node *processMULTIPLEXER(FILE *Circuits, Node *tNode)
{
	int grayCode, index, numVARS, muxSelectors, idx1, *outs, *selectors;
	char iVAR, muxOutput;

	numVARS = 0; muxSelectors = 0;

	fscanf(Circuits, "%i",&numVARS);

	idx1 = numVARS;
	while (idx1 >>= 1)
		++muxSelectors;
	outs = (int*) malloc(sizeof(int) * numVARS);
	selectors=(int*) malloc(sizeof(int)*muxSelectors);

	for (idx1 = 0; idx1 < numVARS; idx1++)
	{
		fscanf(Circuits, " ");
		fscanf(Circuits, "%c", &iVAR);
		outs[idx1] = iVAR;
	}

	for (idx1 = 0; idx1 < muxSelectors; idx1++)
	{
		fscanf(Circuits, " ");
		fscanf(Circuits, "%c", &iVAR);
		if (iVAR == OFF)
			selectors[idx1] = 0;
		else if (iVAR == ON)
			selectors[idx1] = 1;
		else{
			selectors[idx1] = searchNode(tNode, iVAR);
		}
	}

	fscanf(Circuits, " ");
	fscanf(Circuits, "%c", &muxOutput);

	grayCode = binaryBits(selectors, muxSelectors);
	index = searchGrayCode(grayCode);
	if (outs[index] == OFF)
		return addNode(tNode, muxOutput, 0);
	else if (outs[index] == ON)
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
	else if(operandA == OFF)
		return 1;
	else if(operandA == ON)
		return 0;

	operand1 = searchNode(tNode, operandA);
	if(operand1 == 0)
		return 1;
	return 0;
}



int searchGrayCode(int grayCode)
{
	int idx1;
	idx1 = grayCode;
	while ( grayCode >>= 1)
		idx1 ^= grayCode;
	return idx1;
}

int *singleBit(int grayCode, int decoderSize)
{
	int *result; int idx1;
	result = (int*) calloc(sizeof(int), decoderSize);
	for (idx1 = 0; idx1 < decoderSize; idx1++)
	{
		result[decoderSize - idx1 - 1]=((grayCode >> idx1) & 1);
	}
	return result;
}

int binaryBits(int *selectors, int numSelectors)
{
	int idx1 = 0, result = 0;
	while (idx1 < numSelectors)
	{
		result |= (selectors[numSelectors - idx1-1] << idx1);
		idx1++;
	}
	return result;
}

/*************************************************************/

Node *createNode(char name, int Flag)
{
	Node *ptr;
	ptr=(Node*) malloc(sizeof(Node));
	ptr->data = Flag;
	ptr->name = name;
	ptr->next = NULL;
	return ptr;
}

Node *createTempNode(char name, int Flag, Node *next)
{
	Node *ptr;
	ptr=(Node*) malloc(sizeof(Node));
	ptr->data = Flag;
	ptr->name = name;
	ptr->next = next;
	return ptr;
}

Node *addNodeFront(Node *head, char name, int Flag)
{
	Node *n=createTempNode(name, Flag, head);
	n->next = head;
	return n;
}

Node *addNode(Node *head, char name, int Flag)
{
	Node *ptr=head;
	Node *toAdd;
	if (ptr == NULL)
		return createNode(name, Flag);
	if (ptr->name == name){
		ptr->data = Flag;
		return head;
	}
	while (ptr->next != NULL){
		if (ptr->next->name == name){
			ptr->next->data=Flag;
			return head;
		}
		ptr = ptr->next;
	}
	toAdd=createNode(name, Flag);
	ptr->next=toAdd;
	return head;
}

int searchNode(Node *head, char name)
{
	Node *ptr;
	for (ptr=head; ptr!=NULL; ptr=ptr->next)
		if (ptr->name == name)
			return ptr->data;
	return 9;		/* Anything, just not 0 or 1 */
}

/*TROUBLESHOOTING:  printList a linked list */
void printList(Node *head)
{
	Node *ptr;
	if (head==NULL)
		printf("Nothing to print\n");
	for (ptr=head; ptr!=NULL; ptr=ptr->next){
		if (ptr->next==NULL)
			printf("%c:%i\n",ptr->name,ptr->data);
		else
			printf("%c:%i ",ptr->name, ptr->data);
	}
}
