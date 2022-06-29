function setup()
    AbilitySetPortrait("assets/friendly/bard/icon_shield.png");
    AbilitySetCastType(ABILITY_TARGET_ALL);
    SetCooldown(5);
    SetAbilityRange(40);
end

function casted(x,y,obj,headingx,headingy)
    ApplyEffect(obj,{{trigger = TRIGGER_INSTANT, type = EFFECT_HEAL, numTriggers = 1, value = 50, duration = 1}});
    return true;
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end