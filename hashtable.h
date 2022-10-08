#pragma once
#include <stdint.h>

typedef struct HashItem
{
    uint32_t key;
    char* str;
    void* data;
}HashItem;

typedef struct HashTable
{   
    HashItem* items;
    int numElements;
    int numElementsAllocated;

}HashTable;

#define HASH_TABLE_PREALLOC 32

//https://sites.google.com/site/murmurhash/
uint32_t MurmurHash2(const void* key, int len, uint32_t seed);
uint32_t Hash(const char* str);
HashItem* SearchHashTable(HashTable* h, const char* value);
HashTable* InitHashTable();