#include <stdio.h>
#include <stdbool.h>
#include "mpi.h"

int main(int argc , char * argv[])
{
	int my_rank;
	int p;
	int source;
	int dest;
	MPI_Status status;


	MPI_Init( &argc , &argv );
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
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

		int min = x + (r * (my_rank - 1));
		int max = x + (r * my_rank);

		// * Find Primes in Range
		int i;
		for (i = min; i < max; i++){
			bool isPrime = true;
			if (i <= 1 || i == x){
				continue;
			}
			int divisor;
			for (divisor = 2; divisor <= (i / 2); divisor++){
				if (i % divisor == 0){
					isPrime = false;
					break;
				}
			}
			if (isPrime){
				count++;
			}
		}
		// * Send count to master
		MPI_Send(&count, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
	}
	// ! Master
	else
	{
		// * Get X and Y
		int x, y;
		printf("Range min (x): ");
		scanf("%d", &x);
		printf("Range max (y): ");
		scanf("%d", &y);

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
		printf("Total number of prime numbers between %d and %d is: %d\n", x, y, count);
	}

	MPI_Finalize();
	return 0;
}
