#include "effect.h"

#include "gameobject.h"
#include "shield.h"
#include "augment.h"
#include "encounter.h"
#include "particle.h"

#include <math.h>


bool ProcessEffect(Effect* e, GameObject* from, GameObject* target, bool remove)
{
    if (!target)
        return false;
    
    float v = e->value;

    if (from)
        ProcessItemsOnEffect(from,e,&v);
    float value = v * e->stacks;


    
    //If it's a trigger_const we've already added the value to it
    if (e->trigger == TRIGGER_CONST)
        value = e->value;

    if (!remove)
        for (int i = 0; i < ceil(value/4.0f); i++)
        {
            float x; float y;
            GetCentre_Screen(target,&x,&y);
            AddParticleWithRandomProperties(x,y,EffectColors[e->effectType],0.1f,2.5f,1,1.5f);
        }

        
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
        return Damage(from,target,value*sign,propagateItemEffects);
    }
    if (e->effectType == EFFECT_HURT_PERCENT)
    {
        return Damage(from,target,target->maxHP * (value*sign),propagateItemEffects);
    }
    if (e->effectType == EFFECT_STUN)
    {
        Stun(from,target,value*sign);
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
        if (sign==false)
        {

        }
        AddSpeed(target,value*sign);
    }
    if (e->effectType == EFFECT_SHIELD)
    {
        AttachShield(target,e->duration,value*sign);
    }
    if (e->effectType == EFFECT_ATTACKDAMAGE)
    {
        AddDamage(target,value*sign);
    }
    if (e->effectType == EFFECT_ATTACKSPEED)
    {
        AddAttackSpeed(target,value * sign);
    }
    if (e->effectType == EFFECT_ATTACKRANGE)
    {
        AddAttackRange(target,value * sign);
    }
    if (e->effectType == EFFECT_ADD_MANA)
    {
        AddMana(target,value * sign);
    }
    if (e->effectType == EFFECT_ARMOR)
    {
        AddArmor(target,value * sign);
    }
    if (e->effectType == EFFECT_LIFESTEAL)
    {
        AddLifesteal(target,value * sign);
    }




    if (e->effectType == EFFECT_INVULN)
    {
        MakeInvulnerable(target,e->duration*sign);
    }
    if (e->effectType == EFFECT_CURE)
    {
        for (int i = 0; i < e->value; i++)
        {
            CureEffect(target,e->value,false);
        }
    }

    return false;

}
int CureNamedEffect(GameObject* g, const char* name, int numStacksToRemove)
{
    int numStacksRemoved = 0;
    for (int i = 0; i < MAX_EFFECTS; i++)
    {
        Effect* e = &g->effects[i];
        if (e->name && strcmp(name,e->name)==0)
        {
            for (int j = 0; j < numStacksToRemove; j++)
                if (RemoveEffect(e,g,false))
                    numStacksRemoved++;
                else
                    return numStacksRemoved;
        }
    }
    return numStacksRemoved;
}
void CureEffect(GameObject* g, int numEffects, bool removeAllStacks)
{
    for (int i = 0; i < MAX_EFFECTS; i++)
    {
        Effect* e = &g->effects[i];
        if (GetPlayerOwnedBy(e->from) != GetPlayerOwnedBy(g))
        {
            if (RemoveEffect(e,g,removeAllStacks))
                i--;
            numEffects--;
            if (numEffects <= 0)
                return;
        }
    }
}
void CureAll(GameObject* g)
{
    for (int i = 0; i < MAX_EFFECTS; i++)
    {
        if (RemoveEffect(&g->effects[i],g,true))
            i--;

    }
    //CureEffect(g,MAX_EFFECTS,true);
}
bool EffectIsEnabled(Effect* e)
{
    return e->enabled;
}
bool RemoveEffect(Effect* e, GameObject* from, bool removeAllStacks)
{
    if (!e->enabled)
        return false;

    if (e->trigger == TRIGGER_CONST && from)
    {
        ProcessEffect(e,e->from,from,true);
        e->stacks = 0;
    }

    if (e->canStack && !removeAllStacks)
    {
        e->stacks--;
        if (e->stacks > 0)
            return true;
    }
    e->enabled = false;
    e->stacks = 0;
    if (e->name)
        free(e->name);
    if (e->description)
        free(e->description);
    e->description = NULL;
    e->name = NULL;
    e->enabled = false;

    if (from)
    {
        for (int i = e - from->effects; i < MAX_EFFECTS-1; i++)
        {
            from->effects[i] = from->effects[i+1];
        }

    }
    return true;

}
void ProcessEffects(GameObject* g, float dt)
{
    for (int i = 0; i < MAX_EFFECTS; i++)
    {
        Effect* e = &g->effects[i];

        if (e->enabled == true)
        {   
            if (e->trigger != TRIGGER_PERMANENT)
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
                    if (RemoveEffect(e,g,false))
                    {
                        i--;
                        continue;
                    }
                }
            }
            if (e->trigger == TRIGGER_CONST)
            {
                if (e->timer > e->duration)
                {
                    if (RemoveEffect(e,g,false))
                    {
                        i--;
                        continue;
                    }
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
    Effect eNew = CopyEffect(e);
    e = &eNew;
    
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
                        if (e2->stacks < e2->maxStacks)
                        {
                            //e2->value += e->value;
                            e2->stacks++;
                        }
                        e2->duration = e->duration;
                        e2->tickTime = e->tickTime;
                        e2->numTriggers = e->numTriggers;
                        if (e2->trigger == TRIGGER_CONST)
                        {
                            (ProcessEffect(e,from,target,false));
                            e2->value += e->value;
                        }
                        else
                            e2->value = e->value;
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
                
                if (e2->enabled && e->name && e2->name)
                {
                    if (strcmp(e->name,e2->name) == 0)
                    {
                        char* name = e2->name;
                        char* desc = e2->description;

                        memcpy(e2,e,sizeof(Effect));
                        e2->enabled = true;
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
    RemoveEffect(e,NULL,false);
}
