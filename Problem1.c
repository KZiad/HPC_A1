#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc , char * argv[])
{
	int my_rank;		/* rank of process	*/
	int p;			    /* number of process*/
	int source;		    /* rank of sender	*/
	int dest;		    /* rank of receiver	*/
	MPI_Status status;	/* return status for receive*/


	/* Start up MPI */
	MPI_Init( &argc , &argv );

	/* Find out process rank */
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	/* Find out number of process */
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	int count = 0;

	// ! Slave
	if( my_rank != 0)
	{
		dest = 0;
        // * Receive R and X and calculate min and max
		int r, x;
		MPI_Recv(&r, 1, MPI_INT, dest, 0, MPI_COMM_WORLD, &status );
		MPI_Recv(&x, 1, MPI_INT, dest, 1, MPI_COMM_WORLD, &status );

		int min = x + (r * my_rank - 1);
		int max = x + (r * my_rank);

		// TODO: count primes in range
		// TEMP: count will be rank + 1
		count = my_rank + 1;

		// * Send count to master
		MPI_Send(&count, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
	}
	// ! Master
	else
	{
		// TODO: Get X and Y
		// TEMP: X and Y set to test values;
		int x = 1, y = 100;

		// * Send R and X
		int r = (y - x) / (p - 1);
		for( source = 1; source < p ; source++)
		{
			MPI_Send(&r, 1, MPI_INT, source, 0, MPI_COMM_WORLD);
			MPI_Send(&x, 1, MPI_INT, source, 1, MPI_COMM_WORLD);
		}

		// * Receive prime counts
		for( source = 1; source < p ; source++){
			int tempCount;
			MPI_Recv(&tempCount, 1, MPI_INT, source, 0, MPI_COMM_WORLD, &status);
			printf("Total number of prime numbers in P%d is: %d\n", source, tempCount);
			count += tempCount;
		}
		printf("Total number of prime numbers is: %d\n" , count);
	}

	MPI_Finalize();
	return 0;
}
