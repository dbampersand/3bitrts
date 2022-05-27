#pragma once

typedef struct Shield
{
    float duration;
    float amtLeft;
} Shield;

typedef struct GameObject GameObject;
float GetTotalShield(GameObject* g);
void AttachShield(GameObject* g, float duration, float value);
void RemoveShield(GameObject* g, Shield* s);
void ProcessShields(GameObject* g, float dt);
float DamageShields(GameObject* g, float damage);