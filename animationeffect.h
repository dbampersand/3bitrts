#pragma once

#include "rect.h"

typedef struct GameObject GameObject;
typedef struct Sprite Sprite;
typedef enum ANIMATION_EFFECT_PROPERTIES
{
    ANIMATION_EFFECT_ENABLED = 1,
    ANIMATION_EFFECT_OWNED_BY = 2
} ANIMATION_EFFECT_PROPERTIES;

typedef struct AnimationEffect
{
    int sprite_index;
    Rect rect;
    int currIndex;
    float timer;
    float cd;
    int x; int y;
    GameObject* attachedTo;
    char properties;
    int numFrames;

    char* name;
    char* path;
} AnimationEffect;

void DrawAnimationEffects();
void DrawAnimationEffect(AnimationEffect* a);
void InitAnimationEffects();
void ProcessAnimationEffects(float dt);
void AddAnimationEffect(char* path, int x, int y, int w, int h, float cd, bool ownedBy);
void AddAnimationEffect_Prefab(AnimationEffect* animEffect, bool ownedBy, int x, int y);

int AddAnimationEffectPrefab(char* path, int w, int h, float cd);

#define MAX_ANIMATIONEFFECTS 64

extern AnimationEffect animationEffects[MAX_ANIMATIONEFFECTS];
extern int animationEffect_TOP;
extern AnimationEffect* animationEffectsPrefabs;
extern int numAnimationEffectsPrefabs;
extern int allocatedEffectPrefabs;
