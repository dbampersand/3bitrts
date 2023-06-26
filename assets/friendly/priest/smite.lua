local manaTotal = 20
local duration = 8
local tickRate = 0.2

function setup()
    AbilitySetPortrait("assets/friendly/priest/icon_smite.png");
    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityRange(60)
    SetDescription("Smite\n\nHits the target for minor damage. Regenerates mana.")
    SetCooldown(10);
    SetManaCost(10)
    SetAbilityMaxStacks(3);
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/priest/audio/smite.wav",1)

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT
    f1["type"] = EFFECT_HURT
    f1["value"] = 90;  

    local f2 = {}
    f2["trigger"] = TRIGGER_TIMER
    f2["type"] = EFFECT_ADD_MANA
    f2["value"] = manaTotal / duration * tickRate;  
    f2["triggersPerSecond"] = tickRate*100;
    f2["duration"] = duration;  
    f2["name"] = "Smite";


    ApplyEffect(obj,{f1})
    ApplyEffect(GetObjRef(),{f2})
    ShakeScreen(1,0.1)


    return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end