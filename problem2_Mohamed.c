#include <stdio.h>
#include <string.h>
#include "mpi.h"

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

    int type, r;
    char string[100], modifiedString[100], result[100];

    if(my_rank != 0){
        
        MPI_Recv(string, 100, MPI_CHAR, 0, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(&r, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
                
        int a = (my_rank - 1) * r, b = r + a;
        
        b = (b > strlen(string)) ? strlen(string) : b;

        for (int i = a; i < b; i++)
        {
            modifiedString[i] = (char)((int)string[i] + 3);
        }

        MPI_Send(modifiedString, 100, MPI_CHAR, 0, tag, MPI_COMM_WORLD);
    }

    else{
        printf("Input string: ");
        scanf("%s", string);

        // printf("Encryption (1) / Decryption (2)\n");
        // scanf("%d", &type);

        r = strlen(string) / (p - 1);
        
        for (int i = 1; i < p; i++)
        {
            MPI_Send(string, 100, MPI_CHAR, i, tag, MPI_COMM_WORLD);
            MPI_Send(&r, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
        }

        for (int i = 1; i < p; i++)
        {
            MPI_Recv(modifiedString, 100, MPI_CHAR, i, tag, MPI_COMM_WORLD, &status);
            strcat(result, modifiedString);
        }

        printf("Result: %s\n", result);
    }

    MPI_Finalize();
}
