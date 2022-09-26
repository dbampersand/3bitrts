#pragma once



typedef struct Item
{
    int luafunc_update;
    int luafunc_onattack;
    int luafunc_onabilitycast;
    int luafunc_setup;    

    char* description;
    char* name;
} Item;

