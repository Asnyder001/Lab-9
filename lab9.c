#include <stdio.h>
#include <stdlib.h> // Include this for malloc and exit functions

// RecordType
struct RecordType
{
	int id;
	char name;
	int order;
};

// Fill out this structure
struct HashType
{
    struct RecordType record; // Record at this hash index
    struct HashType* next;    // Pointer to the next record in case of collision
};

#define HASH_SIZE 10 // Define hash table size

// Compute the hash function
int hash(int x)
{
    return x % HASH_SIZE; // Simple modulo hashing for demonstration
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*)malloc(sizeof(struct RecordType) * dataSz);
        // Implement parse data block
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
        }

        fclose(inFile);
    }

    return dataSz;
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
    int i;

    for (i = 0; i < hashSz; ++i)
    {
        printf("Index %d -> ", i);
        struct HashType* current = &pHashArray[i];
        while (current != NULL)
        {
            printf("%d %c %d -> ", current->record.id, current->record.name, current->record.order);
            current = current->next;
        }
        printf("\n");
    }
}

int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;

    recordSz = parseData("input.txt", &pRecords);
    // printRecords(pRecords, recordSz); // Commented out since printRecords() is not implemented

    // Initialize hash table
    struct HashType* hashTable[HASH_SIZE];
    for (int i = 0; i < HASH_SIZE; ++i)
    {
        hashTable[i] = NULL;
    }

    // Insert records into hash table
    for (int i = 0; i < recordSz; ++i)
    {
        struct RecordType record = pRecords[i];
        int index = hash(record.id); // Get hash index
        struct HashType* newNode = (struct HashType*)malloc(sizeof(struct HashType)); // Allocate memory for new node
        newNode->record = record;
        newNode->next = NULL;

        // If no collision, directly insert into hash table
        if (hashTable[index] == NULL)
        {
            hashTable[index] = newNode;
        }
        else // Collision handling using separate chaining
        {
            // Traverse to the end of the chain
            struct HashType* current = hashTable[index];
            while (current->next != NULL)
            {
                current = current->next;
            }
            // Append new node to the end
            current->next = newNode;
        }
    }

    // Display records in the hash table
    displayRecordsInHash(hashTable, HASH_SIZE);

    // Free allocated memory
    for (int i = 0; i < HASH_SIZE; ++i)
    {
        struct HashType* current = hashTable[i];
        while (current != NULL)
        {
            struct HashType* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(pRecords);

    return 0;
}
