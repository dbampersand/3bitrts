#pragma once

#include "stdint.h"

#define HASH_TABLE_SIZE 2048    

#define HASH_TABLE_KEY_TYPE uint32_t
#define HASH_TABLE_NUM_BITS (sizeof(HASH_TABLE_KEY_TYPE) * 8)

typedef struct HashData
{
    void* key;
    void* data;
    size_t dataLen;
}HashData;

typedef struct HashKey
{
    int numData;
    HashData* data;
}HashKey;

typedef struct HashTable
{
    HashKey keys[HASH_TABLE_SIZE];
    size_t capacity;
}HashTable;

HASH_TABLE_KEY_TYPE Hash(void* data, size_t dataSize);
void TestHashTable();
HashData* GetFromHashTable(HashTable* ht, const char* str);
int AddToHashTable(HashTable* ht, const char* str, void* data, size_t dataSizeBytes);

