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

    FILE *fptr;
    int type, r, mode;
    char string[100], modifiedString[100], result[100], filename[100];

    if(my_rank != 0){
        
        MPI_Recv(string, 100, MPI_CHAR, 0, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(&r, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(&type, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
                
        int a = (my_rank - 1) * r, b = r + a;
        
        if (my_rank + 1 == p)
        {
            b = (b < strlen(string)) ? strlen(string) : b;
        }

        if(type == 1){
            for (int i = a; i < b && i < strlen(string); i++)
            {
                modifiedString[i - a] = (char)((int)string[i] + 3);
            }
        }
        else{
            for (int i = a; i < b && i < strlen(string); i++)
            {
                modifiedString[i - a] = (char)((int)string[i] - 3);
            }
        }
        MPI_Send(modifiedString, 100, MPI_CHAR, 0, tag, MPI_COMM_WORLD);
    }

    else{
        printf("Read from file (1) or Enter text (2): ");
        scanf("%d", &mode);
        getchar(); // Consume newline character

        if (mode == 1){
            printf("Enter file name: ");
            scanf("%s", filename);
            
            fptr = fopen(filename, "r");
            fgets(string, 100, fptr);
        }
        else{
            printf("Enter text to encrypt/decrypt: ");
            fgets(string, 100, stdin);
        }

        printf("Encryption (1) / Decryption (2)\n");
        scanf("%d", &type);
        getchar(); // Consume newline character


        r = strlen(string) / (p - 1);
        
        for (int i = 1; i < p; i++)
        {
            MPI_Send(string, 100, MPI_CHAR, i, tag, MPI_COMM_WORLD);
            MPI_Send(&r, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
            MPI_Send(&type, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
        }

        memset(result, 0, sizeof(result));

        for (int i = 1; i < p; i++)
        {
            MPI_Recv(modifiedString, 100, MPI_CHAR, i, tag, MPI_COMM_WORLD, &status);
            strcat(result, modifiedString);
        }

        printf("Result: %s\n", result);
    }

    MPI_Finalize();
}
