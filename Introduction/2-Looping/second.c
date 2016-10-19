/*
 ============================================================================
 Name        : second.c
 Author      : Nicole Yson
 Version     : 1.0
 Description : Prime numbers
 ============================================================================
 */


#include<stdio.h>

int main ( int argn, char *argv[] )
{
        if (argn != 2)
        {
                printf ("error\n");
        }
        else
        {
                int num2;
                char* num1;
                num1 = argv[1];
                num2 = atoi(num1);
                if ( isPrime(num2) == 1 )
                        printf("yes\n");
                else
                        printf("no\n");
           return 0;
        }
	return 0;
}

int isPrime(int num2)
{
   int n;

   for ( n = 2 ; n <= num2 - 1 ; n++ )
   {
      if ( num2%n == 0 )
         return 0;
   }
   return 1;
}
