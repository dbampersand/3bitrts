local armor = -3;

function setup()
    AbilitySetPortrait("assets/friendly/lancer/icon_shield_breaker.png");
    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityRange(50)
    SetDescription("Shield Breaker\n\nLowers an enemies' armour.")

    SetCooldown(15);
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/lancer/audio/shield_breaker.wav",0.5)
    
    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_ARMOR;
    f1["value"] = armor;  

    ApplyEffect(obj,{f1})

    ShakeScreen(1,0.1)

    return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end