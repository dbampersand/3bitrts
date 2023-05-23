#include "augment.h"

#include "effect.h"
#include "encounter.h"
#include "attack.h"
#include "colors.h"
#include "gameobject.h"
#include "helperfuncs.h"
#include "attack.h"

#include <stdio.h>
#include <stdlib.h>
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro.h"
#include "rect.h"

Augment AllAugments[AUGMENT_ALL] = {0};

void InitAugments()
{
    for (int i = 0; i < AUGMENT_ALL; i++)
    {
        AllAugments[i].augment = i;
        
        if (i > AUGMENT_NULL && i < AUGMENT_NEUTRAL_DIVIDER)
        {
            AllAugments[i].friendliness = AUGMENT_BAD;
        }
        if (i > AUGMENT_NEUTRAL_DIVIDER && i < AUGMENT_GOOD_DIVIDER)
        {
            AllAugments[i].friendliness = AUGMENT_NEUTRAL;
        }
        if (i > AUGMENT_GOOD_DIVIDER && i < AUGMENT_ALL)
        {
            AllAugments[i].friendliness = AUGMENT_GOOD;
        }
        AllAugments[i].timer = 0;
        AllAugments[i].portrait_sprite_index = 0;
    }

    AllAugments[AUGMENT_BAD_EFFECT_TIME].portrait_sprite_index = LoadSprite("assets/ui/augments/bad/AUGMENT_BAD_EFFECT_TIME.png",true);
    AllAugments[AUGMENT_BAD_RANDOMDMGPOOLS].portrait_sprite_index = LoadSprite("assets/ui/augments/bad/AUGMENT_BAD_RANDOMDMGPOOLS.png",true);
    AllAugments[AUGMENT_BAD_DEATHINCDMG].portrait_sprite_index = LoadSprite("assets/ui/augments/bad/AUGMENT_BAD_DEATHINCDMG.png",true);
    AllAugments[AUGMENT_BAD_MOVESPEED].portrait_sprite_index = LoadSprite("assets/ui/augments/bad/AUGMENT_BAD_MOVESPEED.png",true);
    AllAugments[AUGMENT_BAD_ENEMY_EXPLODES].portrait_sprite_index = LoadSprite("assets/ui/augments/bad/AUGMENT_BAD_ENEMY_EXPLODES.png",true);
    AllAugments[AUGMENT_NEUTRAL_TOTALHEAL].portrait_sprite_index = LoadSprite("assets/ui/augments/neutral/AUGMENT_NEUTRAL_TOTALHEAL.png",true);
    AllAugments[AUGMENT_NEUTRAL_TOTALDAMAGE].portrait_sprite_index = LoadSprite("assets/ui/augments/neutral/AUGMENT_NEUTRAL_TOTALDAMAGE.png",true);
    AllAugments[AUGMENT_GOOD_HEALS].portrait_sprite_index = LoadSprite("assets/ui/augments/good/AUGMENT_GOOD_HEALS.png",true);
    AllAugments[AUGMENT_GOOD_DAMAGE].portrait_sprite_index = LoadSprite("assets/ui/augments/good/AUGMENT_GOOD_DAMAGE.png",true);
    AllAugments[AUGMENT_GOOD_MOVESPEED].portrait_sprite_index = LoadSprite("assets/ui/augments/good/AUGMENT_GOOD_MOVESPEED.png",true);
    AllAugments[AUGMENT_GOOD_SHIELD].portrait_sprite_index = LoadSprite("assets/ui/augments/good/AUGMENT_GOOD_SHIELD.png",true);

}
float GetAugmentDamageBonus(int damage, int augmentLevel)
{
    return damage*(augmentLevel/4.0f);
}
float GetAugmentHealthBonus(int health, int augmentLevel)
{
    return health*(augmentLevel/20.0f);
}
float GetAugmentAbilityDamage(int damage, int augmentLevel)
{
    return damage * (augmentLevel/35.0f);
}

float Neutral_GetAugmentAbilityDamage(int damage, int augmentlevel)
{
    return damage*(augmentlevel/20.0f);
}
float Neutral_GetAugmentAbilityHeal(int damage, int augmentlevel)
{
    return damage*(augmentlevel/20.0f);
}
float Good_GetAugmentAbilityHeal(int heal, int augmentlevel)
{
    return heal*(augmentlevel/20.0f);
}
float Good_GetAugmentAbilityDamage(int damage, int augmentlevel)
{
    return damage*(augmentlevel/20.0f);
}
float Good_GetAugmentMoveSpeed(float movespeed, int augmentlevel)
{
    return movespeed*(0.1f*(float)augmentlevel);
}
float Bad_GetAugmentMoveSpeed(float movespeed, int augmentlevel)
{
    return movespeed*(0.1f*(float)augmentlevel);
}

int GetNumBadAugments(int augmentLevel)
{
    if (augmentLevel <= 2)
        return 0;
    if (augmentLevel <= 4)
        return 1;
    if (augmentLevel <= 6)
        return 2;
    if (augmentLevel <= 8)
        return 3;
    return 3;
}
int GetNumGoodAugments(int augmentLevel)
{
    if (augmentLevel <= 2)
        return 2;
    if (augmentLevel <= 4)
        return 3;
    return 1;

}
float GetAugmentMultiplier(int augmentLevel)
{
    return augmentLevel * 0.15f;
}
int InverseSort(const void* a, const void* b)
{
    return (*(int*)b - *(int*)a);
}
Augment GetRandomAugment(AUGMENT_BUFF_TYPE augType, Encounter* e)
{
    int min; int max;
    if (augType == AUGMENT_GOOD)
    {
        min = AUGMENT_GOOD_DIVIDER;
        max = AUGMENT_ALL;
    }
    else if (augType == AUGMENT_BAD)
    {
        min = AUGMENT_NULL;
         max = AUGMENT_NEUTRAL_DIVIDER;
    }
    else if (augType == AUGMENT_NEUTRAL)
    {
         min = AUGMENT_NEUTRAL_DIVIDER;
         max = AUGMENT_GOOD_DIVIDER;
    }
    else
    {
        Augment a = {0};
        a.augment = AUGMENT_NULL;
        a.friendliness = AUGMENT_NEUTRAL;
        return a;
    }

    int* allAugs = malloc(sizeof(int)*(AUGMENT_ALL+1));
    memset(allAugs,-1,sizeof(int)*(AUGMENT_ALL+1));
    for (int i = min; i < max; i++)
    {
        allAugs[i] = i; 
    }
    allAugs[AUGMENT_NULL] = -1;
    allAugs[AUGMENT_NEUTRAL_DIVIDER] = -1;
    allAugs[AUGMENT_GOOD_DIVIDER] = -1;
    allAugs[AUGMENT_ALL] = -1;

    //remove the ones we already have to make a pool 
    for (int i = 0; i < MAX_AUGMENTS; i++)
    {
        AUGMENT_TYPES a = e->augments[i].augment;
        if (a >= 0 && a < AUGMENT_ALL)
            allAugs[(int)a] = -1;
    }

    qsort(allAugs, AUGMENT_ALL, sizeof(int), InverseSort);
    
    int maxIndex = -1;
    for (int i = 0; i < AUGMENT_ALL; i++)
    {
        if (allAugs[i] == -1)
        {
            maxIndex = i; 
            break;
        }
    }

    //our augment is between 0-maxIndex
    AUGMENT_TYPES selected = allAugs[(rand() % ((maxIndex)))];

    Augment a = AllAugments[selected];
    //a.augment = selected;
    //a.friendliness = augType;


    free(allAugs);

    return a;
}

void ProcessAugments(Augment* a, float dt)
{
    if (!a)
        return;
    for (int i = 0; i < MAX_AUGMENTS; i++)
    {
        a[i].timer += dt;
        if (a[i].augment == AUGMENT_NULL)
            continue;
        if (a[i].augment == AUGMENT_BAD_RANDOMDMGPOOLS)
            Augment_RandomDmgPool(&a[i],currEncounterRunning->augment);
    }
}
bool HasAugment(Encounter* e, AUGMENT_TYPES augment)
{
    for (int i = 0; i < MAX_AUGMENTS; i++)
    {
        if (e->augments[i].augment == augment)
        {
            return true;
        }
    }
    return false;
}
void Augment_ChangeEffectTime(Effect* e, int augmentLevel)
{
    float multiplier = augmentLevel/10.0f;
    e->duration += e->duration * multiplier;
    e->tickTime -= e->tickTime * multiplier;
    e->numTriggers = e->numTriggers * multiplier;
}
void Augment_RandomDmgPool(Augment* a, int augmentLevel)
{
    if (a->timer >  50 / ((float)augmentLevel+1))
    {
        a->timer = 0;


        Effect e = {0}; 
        e.trigger = TRIGGER_TIMER;
        e.effectType = EFFECT_HURT;
        e.from = NULL;
        e.value = augmentLevel/2.0f;
        e.numTriggers = 1;
        e.duration = 1;
        e.enabled = true;
        

        Attack at = {0};
        at.x = rand() % 255;
        at.y = rand() % 255;
        at.targx = at.x;
        at.targy = at.y;
        at.effects = calloc(1,sizeof(Effect));
        at.effects[0] = e;
        at.numEffects = 1;
        at.attackType = ATTACK_AOE;
        at.ownedBy = NULL;
        at.speed = 0;
        at.shouldCallback = false;
        at.duration = augmentLevel * 2;
        at.tickrate = 0.25f;
        at.timer = 0;
        at.range = augmentLevel * 4;
        at.cameFrom = NULL;
        at.color = COLOR_ENEMY;
        at.dither = DITHER_HALF;

        AddAttack(&at);
    }
}
void ClearAugments(Encounter* e)
{
    memset(e->augments,0,sizeof(Augment) * MAX_AUGMENTS);
}
void SetEncounterRandAugments(Encounter* e)
{
    if (!e) return;
    ClearAugments(e);

    int index = 0;
    for (; index < GetNumGoodAugments(e->augment);)
    {
        if (index <= MAX_AUGMENTS)
        {
            e->augments[index] = GetRandomAugment(AUGMENT_GOOD,e);
            index++;
        }
        else break;
    }
    int numGood = index;
    for (; index < numGood+GetNumBadAugments(e->augment);)
    {
        if (index <= MAX_AUGMENTS)
        {
            e->augments[index] = GetRandomAugment(AUGMENT_BAD,e);
            index++;
        }
        else break;
    }
    int numNeutrals = MAX_AUGMENTS - (index);
    for (int i = 0; i < numNeutrals; i++)
    {
        if (index <= MAX_AUGMENTS)
        {
            e->augments[index] = GetRandomAugment(AUGMENT_NEUTRAL,e);
            index++;
        }
        else break;
    }


}
char* GetAugmentDescription(AUGMENT_TYPES aug)
{

    //BAD EFFECTS
    if (aug == AUGMENT_BAD_EFFECT_TIME)
    {
        return "Enemy effects last longer";
    }
    if (aug == AUGMENT_BAD_RANDOMDMGPOOLS)
    {
        return "Random damage pools appear";
    }
    if (aug == AUGMENT_BAD_DEATHINCDMG)
    {
        return "Death adds bonus damage to nearby units";
    }
    if (aug == AUGMENT_BAD_MOVESPEED)
    {
        return "Adds move speed";
    }
    if (aug == AUGMENT_BAD_ENEMY_EXPLODES)
    {
        return "Enemies explode on death";
    }

    //NEUTRAL EFFECTS
    if (aug == AUGMENT_NEUTRAL_TOTALHEAL)
    {
        return "All healing increased";
    }
    if (aug == AUGMENT_NEUTRAL_TOTALDAMAGE)
    {
        return "All damage increased";
    }

    //GOOD EFFECTS
    if (aug == AUGMENT_GOOD_HEALS)
    {
        return "Your healing increased";
    }
    if (aug == AUGMENT_GOOD_DAMAGE)
    {
        return "Your damage increased";
    }
    if (aug == AUGMENT_GOOD_MOVESPEED)
    {
        return "Your movespeed increased";
    }
        if (aug == AUGMENT_GOOD_SHIELD)
    {
        return "Provides a shield. **unimplemented**";
    }



    if (aug == AUGMENT_GOOD_DIVIDER)
    {
        return "Good effect divider (Bug)";
    }
    if (aug == AUGMENT_ALL)
    {
        return "All divider (Bug)";
    }
    if (aug == AUGMENT_NEUTRAL_DIVIDER)
    {
        return "Neutral divider (Bug)";
    }
    return "No Text (bug)";
}
void Bad_AugmentDeathAddDamage(GameObject* g, int augmentLevel)
{
    for (int i = 0; i < numActiveObjects; i++)
    {
        GameObject* g2 = activeObjects[i];
        if (GetDist(g,g2) < 15)
        {
            if (GetPlayerOwnedBy(g) == GetPlayerOwnedBy(g2))
                AddDamage(g2,10 * augmentLevel);
        }
    }
}

ALLEGRO_COLOR* GetAugmentDescriptionColor(Augment* a)
{
    if (a->friendliness == AUGMENT_GOOD)
    {
        return &FRIENDLY;
    }
        if (a->friendliness == AUGMENT_NEUTRAL)
    {
        return &GROUND;
    }
        if (a->friendliness == AUGMENT_BAD)
    {
        return &ENEMY;
    }
    return NULL;
}
void Bad_EnemyExplodes(GameObject* g, int augmentLevel)
{
    float x; float y; 
    GetCentre(g,&x,&y);
    Effect e;
    e.trigger = TRIGGER_TIMER;
    e.effectType = EFFECT_HURT;
    e.from = NULL;
    e.value = augmentLevel * 5;
    e.numTriggers = 1;
    e.timer = 0;
    e.duration = 0;
    e.tickTime = 0;
    e.enabled = false;
    e.spriteIndex_Portrait = 0;
    e.name = NULL;

    CreateAoE(x,y, NULL, 10, 3, 1,false, ATTACK_HITS_FRIENDLIES, ALColorToCol(ENEMY), DITHER_VERTICAL_EIGTH, 1, &e, NULL, NULL);
}
ALLEGRO_COLOR GetAugmentColor(Augment* a)
{
    if (a->friendliness == AUGMENT_GOOD)
        return FRIENDLY;

    if (a->friendliness == AUGMENT_BAD)
        return ENEMY;

    if (a->friendliness == AUGMENT_NEUTRAL)
        return GROUND;
    return BG;
}
bool DrawAugmentPortrait(Augment* a, int x, int y, MouseState* mouseState, ALLEGRO_COLOR col)
{
    int w = 20; int h = 20;
    //ALLEGRO_COLOR col = GetAugmentColor(a);


    Rect r = (Rect){x,y,w,h};
    bool mousedOver = PointInRect(mouseState->screenX,mouseState->screenY,r);
    DrawSprite(&sprites[a->portrait_sprite_index],x,y,0,0,0,col,mousedOver,false,false);
    if (!mousedOver)
        al_draw_rectangle(x,y,x+w,y+h,col,1);
    return mousedOver;

    
}