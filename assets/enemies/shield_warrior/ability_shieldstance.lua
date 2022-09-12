
function setup()

    SetAbilityRange(60)
    --AbilitySetCastType(ABILITY_CONE);
    SetCooldown(40); 
    AbilitySetPortrait("assets/enemies/shield_warrior/ability_shieldstance.png");
    SetDescription("[b]Shield Stance\n\nProvides a large shield.")

end
function casted(x,y,obj,headingx,headingy)
    Print("bbbbb")

    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_SHIELD;
    f1["value"] = 600;  
    f1["duration"] = 20
    ApplyEffect(GetObjRef(),{f1});

    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
end