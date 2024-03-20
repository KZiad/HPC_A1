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

        
	}
	// ! Master
	else
	{
        // TODO: Get Mode
        // TODO: Get encode or decode
        // TODO: Get filename OR string
        // TODO: Split string evenly and send to slaves
        // TODO: Receive results from slaves and print

	}

	MPI_Finalize();
	return 0;
}
