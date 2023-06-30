local heal = 70

local duration = 12
local tickRate = 0.5
local maxStacks = 4

function setup()
    SetAbilityRange(64) 
    SetCooldown(2);
    AbilitySetCastType(ABILITY_TARGET_FRIENDLY);

    AbilitySetPortrait("assets/summoned/woven_dove/ability_cure.png");
    SetDescription("[b]Cure\n\nHeals the target over time, stacking "..maxStacks .. " times.");
    SetAbilityName("Cure"); 

end

function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/summoned/woven_dove/audio/cure.wav",0.5)

    local f1 = {};
    f1["trigger"] = TRIGGER_TIMER
    f1["type"] = EFFECT_HEAL
    f1["duration"] = duration
    f1["value"] = heal * tickRate / duration;

    ApplyEffect(obj,{f1})


    return true; 
end

function onhit(x,y,objhit)
end
function ontimeout(x,y,obj,dt,target)
end
function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
end
function onchanneled() 
end