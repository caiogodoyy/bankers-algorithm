#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int numResources, numCustomers = 1;
int *available;
int **maximum;
int **allocation;
int **need;

int safetyAlgorithm();
int request(int customer, int resources[numResources]);
void release(int customer, int resources[numResources]);

int main(int argc, char const *argv[])
{
    // obtendo os valores de numResources e numCustomers
    numResources = argc - 1;
    FILE *f1 = fopen("customer.txt", "r");
    while (!feof(f1))
    {
        char ch = fgetc(f1);
        if (ch == '\n')
            numCustomers++;
    }
    fclose(f1);

    // alocando espaço na memória
    available = (int *)malloc(numResources * sizeof(int));
    maximum = (int **)malloc(numCustomers * sizeof(int *));
    for (int i = 0; i < numCustomers; i++)
        maximum[i] = (int *)malloc(numResources * sizeof(int));
    allocation = (int **)malloc(numCustomers * sizeof(int *));
    for (int i = 0; i < numCustomers; i++)
        allocation[i] = (int *)malloc(numResources * sizeof(int));
    need = (int **)malloc(numCustomers * sizeof(int *));
    for (int i = 0; i < numCustomers; i++)
        need[i] = (int *)malloc(numResources * sizeof(int));

    // preenchendo available
    for (int i = 0; i < numResources; i++)
        available[i] = atoi(argv[i + 1]);
    // preenchendo maximum
    FILE *f2 = fopen("customer.txt", "r");
    for (int i = 0; i < numCustomers; i++)
        for (int j = 0; j < numResources; j++)
        {
            fscanf(f2, "%d", &maximum[i][j]);
            fgetc(f2);
        }
    fclose(f2);
    // preenchendo allocation
    for (int i = 0; i < numCustomers; i++)
        for (int j = 0; j < numResources; j++)
            allocation[i][j] = 0;
    // preenchendo need
    for (int i = 0; i < numCustomers; i++)
        for (int j = 0; j < numResources; j++)
            need[i][j] = maximum[i][j] - allocation[i][j];

    // arquivo de saída
    FILE *out = fopen("result.txt", "w");
    // leitura dos comandos
    FILE *f3 = fopen("commands.txt", "r");
    char command[2];
    int customer, resources[numResources];
    while (!feof(f3))
    {
        fscanf(f3, "%s", command);
        if (strcmp(command, "*") != 0)
        {
            fscanf(f3, "%d", &customer);
            for (int i = 0; i < numResources; i++)
                fscanf(f3, "%d", &resources[i]);
            fgetc(f3);
        }
        if (strcmp(command, "RQ") == 0)
        {
            int op = request(customer, resources);
            if (op == 0)
            {
                fprintf(out, "Allocate to customer %d the resources", customer);
                for (int j = 0; j < numResources; j++)
                    fprintf(out, " %d", resources[j]);
                fprintf(out, "\n");
            }
            if (op == -1)
            {
                fprintf(out, "The customer %d request", customer);
                for (int j = 0; j < numResources; j++)
                    fprintf(out, " %d", resources[j]);
                fprintf(out, " was denied because exceed its maximum allocation\n");
            }
            if (op == -2)
            {
                fprintf(out, "The resources");
                for (int j = 0; j < numResources; j++)
                    fprintf(out, " %d", available[j]);
                fprintf(out, " are not enough to customer %d request", customer);
                for (int j = 0; j < numResources; j++)
                    fprintf(out, " %d", resources[j]);
                fprintf(out, "\n");
            }
            if (op == -3)
            {
                fprintf(out, "The customer %d request", customer);
                for (int j = 0; j < numResources; j++)
                    fprintf(out, " %d", resources[j]);
                fprintf(out, " was denied because result in an unsafe state\n");
            }
        }
        else if (strcmp(command, "RL") == 0)
        {
            release(customer, resources);
            fprintf(out, "Release from customer %d the resources", customer);
            for (int j = 0; j < numResources; j++)
                fprintf(out, " %d", resources[j]);
            fprintf(out, "\n");
        }
        else
        {
            fprintf(out, "MAXIMUM ");
            int size = numResources * 2 - 8;
            if(size == 0)
                fprintf(out, "|");
            else if(size > 0) {
                for(int j = 0; j < size; j++)
                    fprintf(out, " ");
                fprintf(out, "|");
            }
            else
                fprintf(out, "|");

            fprintf(out, " ALLOCATION ");
            size = numResources * 2 - 11;
            if(size == 0)
                fprintf(out, "|");
            else if(size > 0) {
                for(int j = 0; j < size; j++)
                    fprintf(out, " ");
                fprintf(out, "|");
            }
            else
                fprintf(out, "|");

            fprintf(out, " NEED\n");

            for (int i = 0; i < numCustomers; i++)
            {
                for (int j = 0; j < numResources; j++)
                    fprintf(out, "%d ", maximum[i][j]);
                if(numResources * 2 < 8)
                    for (int j = 0; j < 8 - numResources * 2; j++)
                        fprintf(out, " ");
                fprintf(out, "| ");
                for (int j = 0; j < numResources; j++)
                    fprintf(out, "%d ", allocation[i][j]);
                if(numResources * 2 < 11)
                    for (int j = 0; j < 11 - numResources * 2; j++)
                        fprintf(out, " ");
                fprintf(out, "| ");
                for (int j = 0; j < numResources; j++)
                    fprintf(out, "%d ", need[i][j]);
                fprintf(out, "\n");
            }
            fprintf(out, "AVAILABLE ");
            for (int j = 0; j < numResources; j++)
                fprintf(out, "%d ", available[j]);
            fprintf(out, "\n");
        }
    }

    fclose(f3);
    fclose(out);

    // liberando alocado espaço na memória
    free(available);
    for (int i = 0; i < numCustomers; i++)
        free(maximum[i]);
    free(maximum);
    for (int i = 0; i < numCustomers; i++)
        free(allocation[i]);
    free(allocation);
    for (int i = 0; i < numCustomers; i++)
        free(need[i]);
    free(need);
    return 0;
}

int request(int customer, int resources[numResources])
{
    for (int i = 0; i < numResources; i++)
        if (need[customer][i] < resources[i])
            return -1;
    for (int i = 0; i < numResources; i++)
        if (available[i] < resources[i])
            return -2;
    for (int i = 0; i < numResources; i++)
    {
        available[i] -= resources[i];
        allocation[customer][i] += resources[i];
        need[customer][i] -= resources[i];
    }
    if (safetyAlgorithm() == -1)
    {
        for (int i = 0; i < numResources; i++)
        {
            available[i] += resources[i];
            allocation[customer][i] -= resources[i];
            need[customer][i] += resources[i];
        }
        return -3;
    }
    return 0;
}
void release(int customer, int resources[numResources])
{
    for (int i = 0; i < numResources; i++)
    {
        available[i] += resources[i];
        allocation[customer][i] -= resources[i];
        need[customer][i] += resources[i];
    }
}
int safetyAlgorithm()
{
    int finish[numCustomers];
    int work[numResources];
    for (int i = 0; i < numCustomers; i++)
        finish[i] = 0;
    for (int i = 0; i < numResources; i++)
        work[i] = available[i];
    int count = 0, j;
    while (count < numCustomers)
    {
        int flag = 0;
        for (int i = 0; i < numCustomers; i++)
        {
            if (finish[i] == 0)
            {
                for (j = 0; j < numResources; j++)
                    if (need[i][j] > work[j])
                        break;
            }
            if (j == numResources)
            {
                for (int k = 0; k < numResources; k++)
                    work[k] += allocation[i][k];
                count++;
                finish[i] = 1;
                flag = 1;
            }
        }
        if (flag == 0)
            return -1;
    }
    return 0;
}
