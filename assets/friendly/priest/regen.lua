
function setup()
    AbilitySetPortrait("assets/friendly/priest/icon_regen.png");
    AbilitySetCastType(ABILITY_TARGET_FRIENDLY);
    SetAbilityRange(120)
    SetDescription("Regen\n\nA heal over time effect.")
    SetManaCost(15)
    SetCooldown(1);
    
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/priest/audio/regen.wav",1)

    local f1 = {};
    f1["name"] = "Regen"
    f1["trigger"] = TRIGGER_TIMER
    f1["type"] = EFFECT_HEAL
    f1["value"] = 45;
    f1["duration"] = 12
    f1["triggersPerSecond"] = 1 
    f1["overwrites"] = true
    --f1["canStack"] = true
    --f1["maxStacks"] = 3
 

    ApplyEffect(obj,{f1})
    return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end