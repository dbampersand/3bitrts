
function setup()
    AbilitySetPortrait("assets/friendly/rogue/icon_shiv.png");
    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityRange(10)
    SetDescription("Poison Dagger\n\nPoisons the target.")
    SetCooldown(10)
end
function casted(x,y,obj,headingx,headingy)

    f1 = {};
    f1["name"] = "Poison"
    f1["canStack"] = true;  
    f1["trigger"] = TRIGGER_TIMER
    f1["type"] = EFFECT_HURT
    f1["value"] = 7;  
    f1["triggersPerSecond"] = 1;  
    f1["duration"] = 25;
    f1["maxStacks"] = 5;


    ApplyEffect(obj,{f1})
    return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end