
function setup()
    AbilitySetPortrait("assets/friendly/priest/icon_greaterheal.png");
    AbilitySetCastType(ABILITY_TARGET_FRIENDLY);
    SetAbilityRange(120)
    SetDescription("Greater Heal\n\nA large, but inefficient heal.")
    
end
function casted(x,y,obj,headingx,headingy)
    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT
    f1["type"] = EFFECT_HEAL
    f1["value"] = 60;  

    ApplyEffect(obj,{f1})
    return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end