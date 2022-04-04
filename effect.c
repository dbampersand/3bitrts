#include "effect.h"
#include "gameobject.h"
#include "shield.h"
void ProcessEffect(Effect* e, GameObject* from, GameObject* target, bool remove)
{
    int sign = 1; 
    if (remove) sign = -1;
    if (e->effectType == EFFECT_MAXHP)
    {
        ModifyMaxHP(target,e->value*sign);
    }
    if (e->effectType == EFFECT_DAMAGE)
    {
        Damage(target,e->value*sign);
    }
    if (e->effectType == EFFECT_HEAL)
    {
        Heal(target,e->value*sign);
    }
    if (e->effectType ==  EFFECT_THREAT)
    {
        AddThreat(from,target,e->value*sign);
    }
    if (e->effectType == EFFECT_SPEED)
    {
        AddSpeed(target,e->value*sign);
    }
    if (e->effectType == EFFECT_SHIELD)
    {
        AttachShield(target,e->duration,e->value*sign);
    }
}
void RemoveEffect(Effect* e, GameObject* from)
{
    e->enabled = false;
    if (e->trigger == TRIGGER_CONST)
    {
        ProcessEffect(e,e->from,from,true);
        /*GameObject* target = from;
        if (e->effectType == EFFECT_MAXHP)
        {
            ModifyMaxHP(target,-e->value);
        }
        if (e->effectType == EFFECT_DAMAGE)
        {
            Damage(target,-e->value);
        }
        if (e->effectType == EFFECT_HEAL)
        {
            Heal(target,-e->value);
        }
        if (e->effectType == EFFECT_THREAT)
        {
            AddThreat(e->from,target,-e->value);
        }
        if (e->effectType == EFFECT_SPEED)
        {
            AddSpeed(target,-e->value);
        }*/
    }

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
                    if (e->timer > e->duration / (e->numTriggers))
                    {
                        e->numTriggers--;
                        e->timer=0;
                        ProcessEffect(&g->effects[i],NULL,g, false);
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
void ApplyEffect(Effect* e, GameObject* from, GameObject* target)
{
    //if (e->trigger == TRIGGER_INSTANT)
    {
        ProcessEffect(e,from,target,false);
        /*if (e->effectType ==  EFFECT_DAMAGE)
        {
            Damage(target,e->value);
        }
        if (e->effectType ==  EFFECT_MAXHP)
        {
            ModifyMaxHP(target,e->value);
        }        
        if (e->effectType ==  EFFECT_HEAL)
        {
            Heal(target,e->value);
        }
        if (e->effectType ==  EFFECT_THREAT)
        {
            AddThreat(from,target,e->value);
        }*/
    }
    //else
    {
        for (int i = 0; i < MAX_EFFECTS; i++)
        {
            if (target->effects[i].enabled == false)
            {
                target->effects[i] = *e;
                target->effects[i].enabled = true;
                if (e->trigger == TRIGGER_CONST)
                {
                    ProcessEffect(e,from,target,false);
                }
                break;
            }
        }
    }
}
