#include "effect.h"
#include "gameobject.h"
#include "shield.h"
bool ProcessEffect(Effect* e, GameObject* from, GameObject* target, bool remove)
{
    int sign = 1; 
    if (remove) sign = -1;
    if (e->effectType == EFFECT_MAXHP)
    {
        ModifyMaxHP(target,e->value*sign);
    }
    if (e->effectType == EFFECT_HURT)
    {
        return Damage(from,target,e->value*sign);

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
    if (e->effectType == EFFECT_DAMAGE)
    {
        AddDamage(target,e->value*sign);
    }
    if (e->effectType == EFFECT_INVULN)
    {
        MakeInvulnerable(target,e->duration*sign);
    }
    return false;

}
void RemoveEffect(Effect* e, GameObject* from)
{
    e->enabled = false;
    if (e->trigger == TRIGGER_CONST)
    {
        ProcessEffect(e,e->from,from,true);
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
void ApplyEffect(Effect* e, GameObject* from, GameObject* target)
{
    if (!e) return;
    if (e->trigger == TRIGGER_INSTANT)
    {
        ProcessEffect(e,from,target,false);
    }
    else
    {
        for (int i = 0; i < MAX_EFFECTS; i++)
        {
            if (target->effects[i].enabled == false)
            {
                target->effects[i] = *e;
                target->effects[i].enabled = true;
                if (e->trigger == TRIGGER_CONST)
                {
                    //processeffect returns true if the object died
                    if (ProcessEffect(e,from,target,false))
                        break;
                }
                break;
            }
        }
    }
}
