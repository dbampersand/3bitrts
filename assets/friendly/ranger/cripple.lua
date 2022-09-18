
function setup()
    AbilitySetPortrait("assets/friendly/ranger/icon_cripple.png");
    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityRange(50)
    SetDescription("Cripple\n\nThrows a bola at the target, causing a slow effect and taunting the target.")
    SetCooldown(15);
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/rogue/audio/backstab.wav",1.25)
    f1 = {};
    f1["trigger"] = TRIGGER_CONST
    f1["type"] = EFFECT_SPEED
    f1["value"] = 70;  
    f1["duration"] = 10

    f2 = {};
    f2["trigger"] = TRIGGER_CONST
    f2["type"] = EFFECT_THREAT
    f2["value"] = 999999;
    f2["duration"] = 10;

    ApplyEffect(obj,{f1,f2})
    return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end