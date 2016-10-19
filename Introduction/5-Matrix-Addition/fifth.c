/*
 ============================================================================
 Name        : fifth.c
 Author      : Nicole Yson
 Version     : 1.0
 Description : Sum of two arrays
 ============================================================================
 */

/* Program to insert in a sorted list */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


/* !!!!! MAIN LOGIC !!!!! */
int main ( int argc, char *argv[] )
{

	if ( argc != 2 )
	{
		printf( "error" );
	}
	else

	{
		FILE *file = fopen( argv[1], "r" );

		if ( file == 0 )
		{
			printf( "error" );
		}
		else
		{
			int i, j, m, n;

			char* delim;
/*			char* M;
			char* N;		*/

			/* Define delimiter */
			delim = "\t";			/* delimiter is tab */

			/* Get array size */
			char line_buffer[BUFSIZ];
			fgets(line_buffer, sizeof(line_buffer), file);

/*			M = strtok (line_buffer, delim);	*/
			m = atoi(strtok (line_buffer, delim));

/*			N = strtok (NULL, delim);			*/
			n = atoi(strtok (NULL, delim));

			/* Define Arrays */
/*			int array1[m][n];
			int array2[m][n];
			int arrayX[m][n];	*/

			int *array1 = (int *)malloc(m * n * sizeof(int));
			int *array2 = (int *)malloc(m * n * sizeof(int));
			int *arrayX = (int *)malloc(m * n * sizeof(int));

			/* Loop counters */


			/* Skip line */
			/* fgets(line_buffer, sizeof(line_buffer), file); */

			/* Load Array 1 */
			for (i = 0; i < m; i++)
			{
				fgets(line_buffer, sizeof(line_buffer), file);
/*				array1[i][0] = atoi(strtok (line_buffer, delim)); 	*/
				*(array1 + i*m) = atoi(strtok (line_buffer, delim));
				for (j = 1; j < n; j++)
				{
/*					array1[i][j] = atoi(strtok (NULL, delim));		*/
					*(array1 + i*m + j) = atoi(strtok (NULL, delim));
				}

			}

			/* Skip line */
			fgets(line_buffer, sizeof(line_buffer), file);

			/* Load Array 2 */
			for (i = 0; i < m; i++)
			{
				fgets(line_buffer, sizeof(line_buffer), file);
/*				array2[i][0] = atoi(strtok (line_buffer, delim));	*/
				*(array2 + i*m) = atoi(strtok (line_buffer, delim));
				for (j = 1; j < n; j++)
				{
/*					array2[i][j] = atoi(strtok (NULL, delim));		*/
					*(array2 + i*m + j) = atoi(strtok (NULL, delim));
				}

			}

			fclose(file);

			/* Add Arrays */

			for (i = 0; i < m; i++)
			{
				for (j = 0; j < n; j++)
				{
/*					arrayX[i][j] = array1[i][j] + array2[i][j];		*/
					*(arrayX + i*m + j) = *(array1 + i*m + j) + *(array2 + i*m + j);
/*					printf ("%d\t",*(arrayX + i*m + j) );	*/
				}
/*				printf ("\n");			*/
			}

			/* Print Arrays */
			for (i = 0; i < m; i++)
			{
/*				printf ( "%d", arrayX[i][0] );		*/
				printf ( "%d", *(arrayX + i*m + 0) );
				for (j = 1; j < n; j++)
				{
/*					printf ( "\t%d", arrayX[i][j] );	*/
					printf ( "\t%d", *(arrayX + i*m + j) );
				}
				printf("\n");
			}

		}
	}

	return 0;
}

