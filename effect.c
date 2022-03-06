#include "effect.h"
#include "gameobject.h"
void ProcessEffect(Effect* e, GameObject* g)
{
    if (e->effectType == EFFECT_MAXHP)
    {
        ModifyMaxHP(g,e->value);
    }
    if (e->effectType == EFFECT_DAMAGE)
    {
        Damage(g,e->value);
    }
    if (e->effectType == EFFECT_HEAL)
    {
        Heal(g,e->value);
    }


}
void RemoveEffect(Effect* e)
{
    e->enabled = false;
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
                    if (e->timer > e->duration - (e->numTriggers*e->tickTime))
                    {
                        e->numTriggers--;
                        ProcessEffect(&g->effects[i],g);
                    }
                }
                else
                {
                    RemoveEffect(e);
                }
            }
        }
    }
}
void ApplyEffect(Effect* e, GameObject* target)
{
    if (e->trigger == TRIGGER_INSTANT)
    {
        if (e->effectType ==  EFFECT_DAMAGE)
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
    }
    else
    {
        for (int i = 0; i < MAX_EFFECTS; i++)
        {
            if (target->effects[i].enabled == false)
            {
                target->effects[i] = *e;
                target->effects[i].enabled = true;
                break;
            }
        }
    }
}
