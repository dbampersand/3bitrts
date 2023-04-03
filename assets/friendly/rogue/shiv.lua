
function setup()
    AbilitySetPortrait("assets/friendly/rogue/icon_shiv.png");
    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityRange(50)
    SetDescription("Shiv\n\nDamages and poisons the target.")
    SetCooldown(10)
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/rogue/audio/shiv.wav",1.25)

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT
    f1["type"] = EFFECT_HURT
    f1["value"] = 10;  

    local f2 = {};
    f2["name"] = "Poison"
    f2["canStack"] = true;  
    f2["trigger"] = TRIGGER_TIMER
    f2["type"] = EFFECT_HURT
    f2["value"] = 7;  
    f2["triggersPerSecond"] = 1;  
    f2["duration"] = 25;
    f2["maxStacks"] = 5;


    ApplyEffect(obj,{f1,f2})
    return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end