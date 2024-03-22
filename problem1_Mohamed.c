#include <stdio.h>
#include "mpi.h"

int countPrime(int x, int y){

    int prime = 0;
    for (int i = x; i < y; i++)
    {
        if(i <= 1){
            continue;
        }
        
        int isPrime = 1;

        for (int j = 2; j < i; j++){
            if (i % j == 0){
                isPrime = 0;
                break;
            }
        }
        if (isPrime == 1){
            prime++;
        }
    }
    return prime;
}

int main(int argc, char* argv[]){

    int my_rank;
    int p;
    int source;
    int dest;
    int tag = 0;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    int x, y, r, total = 0, primes;

    if(my_rank != 0){
        
        MPI_Recv(&x, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(&r, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
        
        int a = ((my_rank - 1) * r) + x, b = r + a;
        primes = countPrime(a, b);
        
        MPI_Send(&primes, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);

        printf("Total number of prime numbers in P%d is: %d\n", my_rank, primes);
    }
    else{

        printf("Input X (lower bound)\n");
        scanf("%d", &x);
        printf("Input Y (upper bound)\n");
        scanf("%d", &y);

        r = (y - x)/ (p - 1);
        
        for (int i = 1; i < p; i++)
        {
            MPI_Send(&x, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
            MPI_Send(&r, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
        }

        for (int i = 1; i < p; i++)
        {
            MPI_Recv(&primes, 1, MPI_INT, i, tag, MPI_COMM_WORLD, &status);
            total += primes;
        }

        printf("Total number of prime numbers is: %d\n", total);
    }

    MPI_Finalize();
}
