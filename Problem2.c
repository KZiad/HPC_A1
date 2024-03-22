#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <mpi.h>

#define MAX_STRING_LENGTH 100

void encrypt(char* text) {
    int i;
    for (i = 0; i < strlen(text); i++) {
        if (isupper(text[i]))
            text[i] = 'A' + (text[i] - 'A' + 3) % 26;
        else if (islower(text[i]))
            text[i] = 'a' + (text[i] - 'a' + 3) % 26;
    }
}

void decrypt(char* text) {
    int i;
    for (i = 0; i < strlen(text); i++) {
        if (isupper(text[i]))
            text[i] = 'A' + (text[i] - 'A' - 3 + 26) % 26;
        else if (islower(text[i]))
            text[i] = 'a' + (text[i] - 'a' - 3 + 26) % 26;
    }
}

int main(int argc, char* argv[]) {
    int my_rank, p;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    char text[MAX_STRING_LENGTH];
    int choice;

    if (my_rank == 0) {
        // Master Process
        printf("Enter text to encrypt/decrypt: ");
        fgets(text, MAX_STRING_LENGTH, stdin);

        // Get user's choice for encryption/decryption
        printf("Enter 1 for encryption or 2 for decryption: ");
        scanf("%d", &choice);
        getchar(); // Consume newline character

        // Broadcast choice to all processes
        for (int i = 1; i < p; i++) {
            MPI_Send(&choice, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }

        // Broadcast text to all processes
        for (int i = 1; i < p; i++) {
            MPI_Send(text, MAX_STRING_LENGTH, MPI_CHAR, i, 1, MPI_COMM_WORLD);
        }
    } else {
        // Slave Processes
        MPI_Recv(&choice, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(text, MAX_STRING_LENGTH, MPI_CHAR, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Master Process
    if (my_rank == 0) {
        // Collect results from all slave processes
        char result[MAX_STRING_LENGTH];
        for (int i = 1; i < p; i++) {
            MPI_Recv(result, MAX_STRING_LENGTH, MPI_CHAR, i, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            // Print result from each process
        }
        printf("Output: %s\n", result);
    } else {
        // Perform encryption or decryption
        if (choice == 1)
            encrypt(text);
        else if (choice == 2)
            decrypt(text);

        // Send processed text back to Master
        MPI_Send(text, MAX_STRING_LENGTH, MPI_CHAR, 0, 2, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}
