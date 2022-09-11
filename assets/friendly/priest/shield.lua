
function setup()
    AbilitySetPortrait("assets/friendly/priest/icon_shield.png");
    AbilitySetCastType(ABILITY_TARGET_FRIENDLY);
    SetAbilityRange(60)
    SetDescription("Shield\n\nShields the target from damage.")
    SetManaCost(30)
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/priest/audio/shield.wav",1)

    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT
    f1["type"] = EFFECT_SHIELD
    f1["value"] = 400;  
    f1["duration"]  = 10

    ApplyEffect(obj,{f1})
    return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end