#pragma once

typedef struct Ability
{
    void (*cast)(struct Ability* self);
    
} Ability;
