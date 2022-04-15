
function setup()
    AbilitySetPortrait("Assets/friendly/Rogue/icon_shiv.png");
    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityRange(10)
    SetDescription("Shiv\n\nDamages the target.")
    SetAbilityCooldown(10)
end
function casted(x,y,obj,headingx,headingy)
    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT
    f1["type"] = EFFECT_HURT
    f1["value"] = 10;  

    ApplyEffect(obj,{f1})
    return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end