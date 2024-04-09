#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CUSTOMERS 5
#define RESOURCES 4
#define MAX_COMMAND_LENGTH 50

// Global arrays for available, maximum, allocation, and need
int available[RESOURCES];
int maximum[CUSTOMERS][RESOURCES];
int allocation[CUSTOMERS][RESOURCES];
int need[CUSTOMERS][RESOURCES];

// Function prototypes
void processRequest(char *command);
void requestResources(int customer, int *request);
void releaseResources(int customer, int *release);
void printDataStructures();

int main(int argc, char *argv[])
{
    // Check if correct number of arguments provided
    if (argc != RESOURCES + 1)
    {
        printf("Type your available resources: example:'./main 3 1 2 1' (when 4 resources)\n", argv[0]);
        return 1;
    }

    // Initialize available resources array
    for (int i = 0; i < RESOURCES; i++)
        available[i] = atoi(argv[i + 1]);

    // Read maximum resource requests from file
    FILE *fp = fopen("max.txt", "r");
    if (fp == NULL)
    {
        printf("Error opening file.\n");
        return 1;
    }
    // Read maximum resource requests for each customer from the file
    for (int i = 0; i < CUSTOMERS; i++)
    {
        for (int j = 0; j < RESOURCES; j++)
        {
            if (fscanf(fp, "%d,", &maximum[i][j]) != 1)
            {
                printf("Error reading file.\n");
                fclose(fp);
                return 1;
            }
        }
    }
    fclose(fp);

    // Calculate initial need array
    for (int i = 0; i < CUSTOMERS; i++)
    {
        for (int j = 0; j < RESOURCES; j++)
        {
            need[i][j] = maximum[i][j];
        }
    }

    // Interactive command processing loop
    char command[MAX_COMMAND_LENGTH];
    while (1)
    {
        printf("\nEnter command: \nRQ <customer> <resources>\nRL <customer> <resources>\n* to output the values: ");
        fgets(command, MAX_COMMAND_LENGTH, stdin);
        command[strcspn(command, "\n")] = '\0'; // Remove trailing newline

        if (strcmp(command, "*") == 0)
        {
            printDataStructures();
        }
        else
        {
            processRequest(command);
        }
    }

    return 0;
}

// Process user command for requesting or releasing resources
void processRequest(char *command)
{
    char *token = strtok(command, " ");
    if (token == NULL)
    {
        printf("Invalid command format.\n");
        return;
    }

    if (strcmp(token, "RQ") == 0)
    {
        int customer, resources[RESOURCES];
        for (int i = 0; i < RESOURCES + 1; i++)
        {
            token = strtok(NULL, " ");
            if (token == NULL)
            {
                printf("Invalid command format.\n");
                return;
            }
            if (i == 0)
                customer = atoi(token);
            else
                resources[i - 1] = atoi(token);
        }
        requestResources(customer, resources);
    }
    else if (strcmp(token, "RL") == 0)
    {
        int customer, resources[RESOURCES];
        for (int i = 0; i < RESOURCES + 1; i++)
        {
            token = strtok(NULL, " ");
            if (token == NULL)
            {
                printf("Invalid command format.\n");
                return;
            }
            if (i == 0)
                customer = atoi(token);
            else
                resources[i - 1] = atoi(token);
        }
        releaseResources(customer, resources);
    }
    else
    {
        printf("Invalid command.\n");
    }
}

// Function to handle resource request
void requestResources(int customer, int *request)
{
    // Add request to allocation array
    for (int i = 0; i < RESOURCES; i++)
    {
        if (request[i] > need[customer][i] || request[i] > available[i])
        {
            printf("Request denied\n");
            return;
        }
    }

    for (int i = 0; i < RESOURCES; i++)
    {
        available[i] -= request[i];
        allocation[customer][i] += request[i];
        need[customer][i] -= request[i];
    }

    printf("Request satisfied.\n");
}

// Function to handle resource release
void releaseResources(int customer, int *release)
{
    // Check if release exceeds allocation
    for (int i = 0; i < RESOURCES; i++)
    {
        if (release[i] > allocation[customer][i])
        {
            printf("Release denied.\n");
            return;
        }
    }

    // Release resources
    for (int i = 0; i < RESOURCES; i++)
    {
        available[i] += release[i];
        allocation[customer][i] -= release[i];
        need[customer][i] += release[i];
    }

    printf("Resources released.\n");
}

// Function to print all data structures
void printDataStructures()
{
    printf("\nAvailable Resources: ");
    for (int i = 0; i < RESOURCES; i++)
        printf("%4d", available[i]);

    printf("\n\nAllocation \t\t\t MAX \t\t\t\tNeed\n");
    for (int i = 0; i < CUSTOMERS; i++)
    {
        printf("Customer %d", (i + 1));
        for (int j = 0; j < RESOURCES; j++)
            printf("%4d", allocation[i][j]);
        printf("\t\t");
        for (int j = 0; j < RESOURCES; j++)
            printf("%4d", maximum[i][j]);
        printf("\t\t");
        for (int j = 0; j < RESOURCES; j++)
            printf("%4d", need[i][j]);
        printf("\n");
    }
}
