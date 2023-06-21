#include "hashtable.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
 HASH_TABLE_KEY_TYPE Hash(void* data, size_t dataSize)
{
    HASH_TABLE_KEY_TYPE hash = 0;
    char* d = (char*)data;
    for (int i = 0; i < dataSize; i++)
    {
        hash += d[i];
        hash += hash << 10;
        hash ^= hash >> 6;
    }
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    return hash;
}
HashData* GetFromHashTable(HashTable* ht, const char* str)
{
    HASH_TABLE_KEY_TYPE hash = Hash((void*)str, strlen(str)) % HASH_TABLE_SIZE;
    HashKey* k = &ht->keys[hash];
    for (int i = 0; i < k->numData; i++)
    {
        HashData* hd = &k->data[i];
        if (!hd->key)
            continue;

        char* str2 = (char*)hd->key;
        if (strcmp(str,str2)==0)
        {
            return hd;
        }
    }
    return NULL;
}

int AddToHashTable(HashTable* ht, const char* str, void* data, size_t dataSizeBytes)
{
    HASH_TABLE_KEY_TYPE hash = Hash((void*)str, strlen(str)) % HASH_TABLE_SIZE;
    HashKey* k = &ht->keys[hash]; 

    k->numData++;

    if (!k->data)
        k->data = calloc(1,sizeof(HashData));
    k->data = realloc(k->data,k->numData * sizeof(HashData));

    HashData* hashdata = &k->data[k->numData-1];

    hashdata->key = calloc(strlen(str)+1,sizeof(char));
    strcpy((char*)hashdata->key,str);

    hashdata->data = calloc(dataSizeBytes,sizeof(char));
    memcpy(hashdata->data,data,dataSizeBytes);
    hashdata->dataLen = dataSizeBytes;

    ht->capacity++;

    return hash;
}

void TestHashTable()
{
    HashTable ht = {0};

    int data[] = {32,123,64,129,6321};

    int hash1 = AddToHashTable(&ht, "Test hash table 1",&data[0],sizeof(data[0]));
    int hash2 = AddToHashTable(&ht, "Test hash table 2",&data[1],sizeof(data[0]));
    int hash3 = AddToHashTable(&ht, "Test hash table 3",&data[2],sizeof(data[0]));
    int hash4 = AddToHashTable(&ht, "Test hash table 4",&data[3],sizeof(data[0]));
    int hash5 = AddToHashTable(&ht, "Test hash table 5",&data[4],sizeof(data[0]));

    HashData* hd = GetFromHashTable(&ht,"Test hash table 1");
    int i = (int)(*(int*)hd->data);
    printf("HASH 1: hash: %i, %i, %s\n",hash1,i,i==data[0]? "passes" : "fails");

    hd = GetFromHashTable(&ht,"Test hash table 2");
    i = (int)(*(int*)hd->data);
    printf("HASH 2: hash: %i, %i, %s\n",hash2,i,i==data[1]? "passes" : "fails");

    hd = GetFromHashTable(&ht,"Test hash table 3");
    i = (int)(*(int*)hd->data);
    printf("HASH 3: hash: %i, %i, %s\n",hash3,i,i==data[2]? "passes" : "fails");

    hd = GetFromHashTable(&ht,"Test hash table 4");
    i = (int)(*(int*)hd->data);
    printf("HASH 4: hash: %i, %i, %s\n",hash4,i,i==data[3]? "passes" : "fails");

    hd = GetFromHashTable(&ht,"Test hash table 5");
    i = (int)(*(int*)hd->data);
    printf("HASH 5: hash: %i, %i, %s\n",hash5,i,i==data[4]? "passes" : "fails");




}