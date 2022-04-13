
function setup()
    AbilitySetPortrait("Assets/friendly/priest/icon_minorheal.png");
    AbilitySetCastType(ABILITY_TARGET_FRIENDLY);
    SetAbilityRange(60)
    SetDescription("Minor Heal\n\nA small, efficient heal.")
    
end
function casted(x,y,obj,headingx,headingy)
    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT
    f1["type"] = EFFECT_HEAL
    f1["value"] = 10;  

    ApplyEffect(obj,{f1})
    return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end