local totalHeal = 1200
local duration = 30
local tickRate = 0.1
local maxStacks = 3
function setup()
    AbilitySetPortrait("assets/friendly/herbalist/icon_regenerative_herbs.png");
    AbilitySetCastType(ABILITY_TARGET_FRIENDLY);
    SetAbilityRange(60)
    SetDescription("Regenerative Herbs\n\nHeal over time effect. Stacks up to " .. maxStacks .. " times.");
    SetCooldown(4);
    SetManaCost(10)
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/herbalist/audio/regenerative_herbs.wav",1.25)

    local f1 = {};
    f1["trigger"] = TRIGGER_TIMER
    f1["type"] = EFFECT_HEAL
    f1["value"] = totalHeal / duration * tickRate;  
    f1["triggersPerSecond"] = tickRate*100;
    f1["duration"] = duration;  
    f1["maxStacks"] = maxStacks;
    f1["canStack"] = true
    f1["name"] = "Regenerative Herbs";

    ApplyEffect(obj,{f1})

    return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end