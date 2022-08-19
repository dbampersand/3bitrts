#include "effect.h"

#include "gameobject.h"
#include "shield.h"
#include "augment.h"
#include "encounter.h"

bool ProcessEffect(Effect* e, GameObject* from, GameObject* target, bool remove)
{
    if (!target)
        return false;
     float value = e->value;
    if (!IsOwnedByPlayer(from))
    {
        if (currEncounterRunning)
            value += GetAugmentAbilityDamage(value,currEncounterRunning->augment);
    }
    int sign = 1; 
    if (remove) sign = -1;
    if (e->effectType == EFFECT_MAXHP)
    {
        ModifyMaxHP(target,value*sign);
    }
    if (e->effectType == EFFECT_HURT)
    {
        return Damage(from,target,value*sign);
    }
    if (e->effectType == EFFECT_HEAL)
    {
        Heal(target,value*sign);
    }
    if (e->effectType ==  EFFECT_THREAT)
    {
        if (from)
            AddThreat(from,target,value*sign);
    }
    if (e->effectType == EFFECT_SPEED)
    {
        AddSpeed(target,value*sign);
    }
    if (e->effectType == EFFECT_SHIELD)
    {
        AttachShield(target,e->duration,value*sign);
    }
    if (e->effectType == EFFECT_DAMAGE)
    {
        AddDamage(target,value*sign);
    }
    if (e->effectType == EFFECT_INVULN)
    {
        MakeInvulnerable(target,e->duration*sign);
    }
    if (e->effectType == EFFECT_CURE)
    {
        for (int i = 0; i < e->value; i++)
        {
            CureEffect(target,e->value);
        }
    }

    return false;

}
void CureEffect(GameObject* g, int numEffects)
{
    for (int i = 0; i < MAX_EFFECTS; i++)
    {
        Effect* e = &g->effects[i];
        if (GetPlayerOwnedBy(e->from) != GetPlayerOwnedBy(g))
        {
            RemoveEffect(e,g);
            numEffects--;
            if (numEffects <= 0)
                return;
        }
    }
}
void CureAll(GameObject* g)
{
    CureEffect(g,MAX_EFFECTS);
}
bool EffectIsEnabled(Effect* e)
{
    return e->enabled;
}
void RemoveEffect(Effect* e, GameObject* from)
{
    if (e->trigger == TRIGGER_CONST && from)
    {
        ProcessEffect(e,e->from,from,true);
    }

    if (e->canStack)
    {
        e->stacks--;
        if (e->stacks > 0)
            return;
    }
    e->enabled = false;
    if (from)
    {
        for (int i = e - from->effects; i < MAX_EFFECTS-1; i++)
        {
            from->effects[i] = from->effects[i+1];
        }

    }
    if (e->name)
        free(e->name);
    if (e->description)
        free(e->description);
    e->description = NULL;
    e->name = NULL;

}
void ProcessEffects(GameObject* g, float dt)
{
    for (int i = 0; i < MAX_EFFECTS; i++)
    {
        Effect* e = &g->effects[i];

        if (e->enabled == true)
        {   
            e->timer += dt;

            if (e->trigger == TRIGGER_TIMER)
            {
                if (e->numTriggers >= 0)
                {
                    if (e->timer > e->tickTime)
                    {
                        e->numTriggers--;
                        e->timer = 0;
                        ProcessEffect(&g->effects[i],g->effects[i].from,g, false);
                    }
                }
                else
                {
                    RemoveEffect(e,g);
                }
            }
            if (e->trigger == TRIGGER_CONST)
            {
                if (e->timer > e->duration)
                {
                    RemoveEffect(e,g);
                }
            }
        }
    }
}
Effect CopyEffect(Effect* e)
{
    Effect e2 = *e;
    if (e->name)
    {
        e2.name = calloc(strlen(e->name)+1,sizeof(char));
        strcpy(e2.name,e->name);
    }
    if (e->description)
    {
        e2.description = calloc(strlen(e->description)+1,sizeof(char));
        strcpy(e2.description,e->description);
    }
    return e2;

}
void ApplyEffect(Effect* e, GameObject* from, GameObject* target)
{
    if (!e) return;
    //make a copy of the effect as otherwise we're using the same pointers for name/desc as the Attack
    //which causes a crash when freed
    //not an elegant solution but it works
    Effect e2 = CopyEffect(e);
    e = &e2;
    
    if (HasAugment(currEncounterRunning,AUGMENT_BAD_EFFECT_TIME))
    {
        Augment_ChangeEffectTime(e,currEncounterRunning->augment);
    }

    if (e->trigger == TRIGGER_INSTANT)
    {
        ProcessEffect(e,from,target,false);
    }
    else
    {
        for (int i = 0; i < MAX_EFFECTS; i++)
        {
            if (e->canStack)
            {
                Effect* e2 = &target->effects[i];
                if (e2->enabled && e2->name && e->name)
                {
                    if (strcmp(e->name,e2->name)==0)
                    {
                        e2->value += e->value;
                        e2->duration = e->duration;
                        e2->stacks++;
                        return;
                    }
                }
            }
        }

        bool found = false;
        for (int i = 0; i < MAX_EFFECTS; i++)
        {
            if (e->overwrites)
            {
                Effect* e2 = &target->effects[i];
                
                if (e->enabled && e->name && e2->name)
                {
                    if (strcmp(e->name,e2->name) == 0)
                    {
                        char* name = e2->name;
                        char* desc = e2->description;

                        memcpy(e2,e,sizeof(Effect));
                        return;
                    }
                }
            }
        }
        for (int i = 0; i < MAX_EFFECTS; i++)
        {
            if (target->effects[i].enabled == false)
            {
                target->effects[i] = *e;
                target->effects[i].enabled = true;
                target->effects[i].stacks = 1;
                if (e->trigger == TRIGGER_CONST)
                {
                    //processeffect returns true if the object died
                    if (ProcessEffect(e,from,target,false))
                        return;
                }
                return;
            }
        }
    }
    //if we get here then we haven't applied it to anything
    RemoveEffect(e,target);
}
