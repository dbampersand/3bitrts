
function setup()
    AbilitySetPortrait("Assets/friendly/Rogue/icon_backstab.png");
    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityRange(60)
    SetDescription("Smite\n\nHits the target for minor damage.")
    SetAbilityCooldown(10);
end
function casted(x,y,obj,headingx,headingy)
    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT
    f1["type"] = EFFECT_DAMAGE
    f1["value"] = 20;  

    ApplyEffect(obj,{f1})
    return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end