local xMin = 60
local xMax = 220

local yMin = 38
local yMax = 220

local atk = -1;


local ticksPerSec = 3
local dps = 15
local numAttacks = 3

function setup()
    SetAbilityRange(20)
    SetCooldown(15);
    AbilitySetPortrait("assets/enemies/cusith/ability_focus.png");
    SetDescription("[b]Focus\n\nLowers movement speed and deals damage in an area.")
    
end

function casted(x,y,obj,headingx,headingy)

    PlaySound("assets/enemies/cusith/audio/focus.wav",1)

    local f1 = {}
    f1["trigger"] = TRIGGER_CONST
    f1["type"] = EFFECT_SPEED
    f1["duration"] = 6
    f1["value"] = -30
    f1["overwrites"] = true
    f1["name"] = "Focus"

    local f2 = {}
    f2["trigger"] = TRIGGER_INSTANT
    f2["type"] = EFFECT_HURT
    f2["value"] = dps / ticksPerSec



    targets = GetObjsByName("Pillar")
    local samples = GetSampleIndices(3,1,#targets);
    local numTargs =  math.min(#targets, numAttacks)
    for i = 1, numTargs do
        local targ = targets[samples[i]]
        atk = CreateAOE(GetX(targ),GetY(targ),"", 50, ticksPerSec, 12, true, ATTACK_HITS_ENEMIES, COLOR_SPEED, DITHER_NONE, false, -1, {f1,f2})
        
    end

    return true;
end

function onhit(x,y,objhit)

end
function ontimeout(x,y,obj,dt,target, attack)
end

function abilitytick(x, y, timeUntilAttackTick, parent, target, dt, attackRef, totalAttackDurationLeft)

end