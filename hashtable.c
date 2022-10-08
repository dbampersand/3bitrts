#include "hashtable.h"
#include "stdint.h"
#include <stdlib.h>
#include <string.h>

uint32_t MurmurHash2(const void* key, int len, uint32_t seed)
{

	const uint32_t m = 0x5bd1e995;
	const uint32_t r = 24;


	uint32_t h = seed ^ len;

	const unsigned char* data = (const unsigned char*)key;

	while(len >= 4) 
	{
		//unsigned int k = *(unsigned int *)data;
        uint32_t k = 0;
        memcpy(&k,data,4);

		k *= m; 
		k ^= k >> r; 
		k *= m; 
		
		h *= m; 
		h ^= k;

		data += 4;
		len -= 4;
	}
	
	switch(len)
	{
        case 3: h ^= data[2] << 16;
        case 2: h ^= data[1] << 8;
        case 1: h ^= data[0];
                h *= m;
	};


	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;

	return h;
} 
uint32_t Hash(const char* str)
{
    int len = strlen(str);
    uint32_t hash = MurmurHash2(str,len,0);
    return hash;
}
HashItem* SearchHashTable(HashTable* h, const char* value)
{
    uint32_t key = Hash(value);
    for (int i = 0; i < h->numElements; i++)
    {
        if (h->items[i].key == key)
        {
            //need to do this incase we have a collision
            if (strcmp(h->items[i].str,value)==0)
                return &h->items[i];
        }
    }
    if (h->numElements+1 >= h->numElementsAllocated)
    {
        h->numElementsAllocated += HASH_TABLE_PREALLOC;
        h->items = realloc(h->items,h->numElementsAllocated*sizeof(HashItem));
        memset(h->items+h->numElements,0,sizeof(HashItem)*HASH_TABLE_PREALLOC);

    }

    HashItem* it = &h->items[h->numElements];
    it->str = calloc(strlen(value)+1,sizeof(char));
    strcpy(it->str,value);
    it->key = key;

    h->numElements++;

    return it;
}
HashTable* InitHashTable()
{
    HashTable* h = calloc(1,sizeof(HashTable));
    h->numElementsAllocated = HASH_TABLE_PREALLOC;
    h->items = calloc(HASH_TABLE_PREALLOC,sizeof(HashItem));
    h->numElements = 0;
    return h;
}