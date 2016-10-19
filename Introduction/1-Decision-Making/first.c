/*
 ============================================================================
 Name        : first.c
 Author      : Nicole Yson
 Version     : 1.0
 Description : Odd and Even numbers
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

int main ( int argc, char *argv[] )
{
        if (argc != 2)
        {
                printf ("error\n");
        }
        else
        {
                int num2;
                char* num1;
                num1 = argv[1];
                num2 = atoi(num1);

                if (num2%2 == 1)
                        printf ("odd\n");
                else
                        printf ("even\n");
        }
	return 0;
}

