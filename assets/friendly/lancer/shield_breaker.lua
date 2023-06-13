local armor = -3;

function setup()
    AbilitySetPortrait("assets/friendly/lancer/icon_shield_breaker.png");
    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityRange(50)
    SetDescription("Shield Breaker\n\nLowers an enemies' armour.")

    SetCooldown(15);
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/rogue/audio/shiv.wav",1.25)
    
    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_ARMOR;
    f1["value"] = armor;  

    ApplyEffect(obj,{f1})


    return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end