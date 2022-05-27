#include "animationeffect.h"
#include "sprite.h"
#include "colors.h"
#include "gameobject.h"
void AddAnimationEffect_Prefab(AnimationEffect* animEffect, bool ownedBy, int x, int y)
{
   AnimationEffect* a = &animationEffects[animationEffect_TOP];
    *a = *animEffect;

    a->x = x;
    a->y = y;
    a->rect.x = 0;
    a->rect.y = 0;
    a->timer = 0;

   if (ownedBy)
        a->properties |= ANIMATION_EFFECT_OWNED_BY;
    else
        a->properties &= ~ANIMATION_EFFECT_OWNED_BY;
    a->properties |= ANIMATION_EFFECT_ENABLED;


    animationEffect_TOP++;
    if (animationEffect_TOP >= MAX_ANIMATIONEFFECTS)
        animationEffect_TOP = 0;


}

int AddAnimationEffectPrefab(char* path, int w, int h, float cd)
{
    for (int i = 0; i < numAnimationEffectsPrefabs; i++)
    {
        if (strcasecmp(path, animationEffectsPrefabs[i].path) == 0)
            return i;
    }

    AnimationEffect a;
    memset(&a,0,sizeof(AnimationEffect));

    a.sprite_index = LoadSprite(path,false);
    if (a.sprite_index == -1)
        return 0;

    a.rect.x = 0;
    a.rect.y = 0;
    a.rect.w = w;
    a.rect.h = h;
    a.cd = cd; 
    a.timer = 0;
    a.path = path;
    Sprite* s = &sprites[a.sprite_index];
    int numColumns = al_get_bitmap_width(s->sprite) / w;
    int numRows = al_get_bitmap_height(s->sprite) / h;
    a.numFrames = numColumns + numRows;    
    a.properties |= ANIMATION_EFFECT_ENABLED;

    animationEffectsPrefabs[numAnimationEffectsPrefabs] = a;
    numAnimationEffectsPrefabs++;
    if (numAnimationEffectsPrefabs >= allocatedEffectPrefabs)
    {
        animationEffectsPrefabs = realloc(animationEffectsPrefabs,allocatedEffectPrefabs+BUFFER_PREALLOC_AMT);
        allocatedEffectPrefabs += BUFFER_PREALLOC_AMT;
    }
    return numAnimationEffectsPrefabs-1;

}
void AddAnimationEffect(char* path, int x, int y, int w, int h, float cd, bool ownedBy)
{
    AnimationEffect* a = &animationEffects[animationEffect_TOP];
    a->sprite_index = LoadSprite(path,false);
    if (a->sprite_index == -1)
        return;

    a->x = x;
    a->y = y;
    a->rect.x = 0;
    a->rect.y = 0;
    a->rect.w = w;
    a->rect.h = h;
    a->cd = cd; 
    a->timer = 0;
    Sprite* s = &sprites[a->sprite_index];
    int numColumns = al_get_bitmap_width(s->sprite) / w;
    int numRows = al_get_bitmap_height(s->sprite) / h;
    a->numFrames = numColumns + numRows;    
    if (ownedBy)
        a->properties |= ANIMATION_EFFECT_OWNED_BY;
    else
        a->properties &= ~ANIMATION_EFFECT_OWNED_BY;
    a->properties |= ANIMATION_EFFECT_ENABLED;

    animationEffect_TOP++;
    if (animationEffect_TOP >= MAX_ANIMATIONEFFECTS)
        animationEffect_TOP = 0;

}
void ProcessAnimationEffects(float dt)
{
    for (int i = 0; i < MAX_ANIMATIONEFFECTS; i++)
    {
        AnimationEffect* a = &animationEffects[i];
        if (!(a->properties & ANIMATION_EFFECT_ENABLED))
            continue;
        if (a->sprite_index > 0)
        {

            a->timer += dt;
            if (a->timer >= a->cd)
            {
                a->timer = 0;
                a->rect.x += a->rect.w;
                if (a->rect.x + a->rect.w >= al_get_bitmap_width(sprites[a->sprite_index].sprite))
                {
                    a->rect.x = 0;
                    a->rect.y += a->rect.h;
                    if (a->rect.y  >= al_get_bitmap_height(sprites[a->sprite_index].sprite))
                    {   
                        a->properties &= ~ANIMATION_EFFECT_ENABLED;
                    }
                }
                else
                {
                    //a->rect.x += a->rect.w;
                }
            }
        }           

    }
}

void InitAnimationEffects()
{
    numAnimationEffectsPrefabs = 0;

    memset(animationEffects,0,sizeof(AnimationEffect)*MAX_ANIMATIONEFFECTS);
    animationEffect_TOP = 0;

    animationEffectsPrefabs = calloc(8,sizeof(AnimationEffect));
    allocatedEffectPrefabs = 8;

    AddAnimationEffectPrefab("assets/ui/slash_fx2.png", 16, 16, 0.05f);
}
void DrawAnimationEffects()
{
    for (int i = 0; i < MAX_ANIMATIONEFFECTS; i++)
    {
        AnimationEffect* a = &animationEffects[i];

        if (a->properties & ANIMATION_EFFECT_ENABLED)
            DrawAnimationEffect(&animationEffects[i]);
    }
}
void DrawAnimationEffect(AnimationEffect* a)
{
    DrawSpriteRegion(&sprites[a->sprite_index],a->rect.x,a->rect.y,a->rect.w,a->rect.h,a->x,a->y,a->properties & ANIMATION_EFFECT_OWNED_BY ? ENEMY : FRIENDLY, false);
}
