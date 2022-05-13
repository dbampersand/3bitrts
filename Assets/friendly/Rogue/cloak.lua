
function setup()
    AbilitySetPortrait("assets/friendly/rogue/icon_cloak.png");
    AbilitySetCastType(ABILITY_INSTANT);
    SetDescription("Cloak\n\nProtects you from damage for 4 seconds.")
    SetAbilityCooldown(30);
end
function casted(x,y,obj,headingx,headingy)
    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT
    f1["type"] = EFFECT_INVULN
    f1["duration"] = 4;
    ApplyEffect(GetObjRef(),{f1})

    return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end