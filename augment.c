#include "augment.h"

int GetAugmentDamageBonus(int damage, int augmentLevel)
{
    return damage*(augmentLevel/10.0f);
}
int GetAugmentHealthBonus(int health, int augmentLevel)
{
    return health*(augmentLevel/20.0f);
}
int GetAugmentAbilityDamage(int damage, int augmentLevel)
{
    return damage * (augmentLevel/35.0f);
}
