
function setup()
    AbilitySetPortrait("Assets/friendly/Rogue/icon_backstab.png");
    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityRange(10)
    SetDescription("Backstab\n\nHits the target for major damage and causes a bleed effect.")
    SetAbilityCooldown(30);
end
function casted(x,y,obj,headingx,headingy)
    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT
    f1["type"] = EFFECT_DAMAGE
    f1["value"] = 40;  

    f2["trigger"] = TRIGGER_TIMER
    f2["type"] = EFFECT_DAMAGE
    f2["value"] = 5;
    f2["duration"] = 10;
    f2["triggersPerSecond"] = 1 

    ApplyEffect(obj,{f1,f2})
    return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end