local duration = 12;
local aoe = -1

local damage = 100

local numCastsOnTarget = 0
local target = -1

local maxStacks = 5

function setup()
    AbilitySetPortrait("assets/friendly/bard/icon_aoe_shield.png");
    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityRange(80)
    SetCooldown(10) 
    SetDescription("Counterbalance\n\nDeals damage proportional to the number of consecutive casts on the same target.")
    
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/spell_weaver/audio/counterbalance.wav",1)

    if (obj ~= target) then
        target = obj
        numCastsOnTarget = 0
    end
    numCastsOnTarget = numCastsOnTarget + 1
    if (numCastsOnTarget >= maxStacks) then
        numCastsOnTarget = maxStacks
    end

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = damage * numCastsOnTarget;

    ApplyEffect(obj,{f1});

    return true;
end
function untoggle()
end


function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
end