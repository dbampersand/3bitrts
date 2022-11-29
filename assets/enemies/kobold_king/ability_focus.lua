local xMin = 60
local xMax = 220

local yMin = 38
local yMax = 220



function setup()
    SetAbilityRange(20)
    SetCooldown(20);
    AbilitySetPortrait("assets/enemies/cusith/ability_focus.png");
    SetDescription("[b]Focus\n\nLowers movement speed in an area.")
    
end

function casted(x,y,obj,headingx,headingy)

    local f1 = {}
    f1["trigger"] = TRIGGER_CONST
    f1["type"] = EFFECT_SPEED
    f1["duration"] = 6
    f1["value"] = -30
    f1["overwrites"] = true
    f1["name"] = "Focus"

    x = RandRange(xMin,xMax);
    y = RandRange(yMin,yMax);

    atk = CreateAOE(x,y,"", 50, 0.1, 12, true, ATTACK_HITS_ENEMIES, COLOR_SPEED, DITHER_NONE, false, GetObjRef(), {f1})
    
    x = RandRange(xMin,xMax);
    y = RandRange(yMin,yMax);

    atk = CreateAOE(x,y,"", 50, 0.1, 12, true, ATTACK_HITS_ENEMIES, COLOR_SPEED, DITHER_NONE, false, GetObjRef(), {f1})
   
    x = RandRange(xMin,xMax);
    y = RandRange(yMin,yMax);

    atk = CreateAOE(x,y,"", 50, 0.1, 12, true, ATTACK_HITS_ENEMIES, COLOR_SPEED, DITHER_NONE, false, GetObjRef(), {f1})
    return true;
end

function onhit(x,y,objhit)

end
function ontimeout(x,y,obj,dt,target, attack)
end

function abilitytick(x, y, timeUntilAttackTick, parent, target, dt, attackRef, totalAttackDurationLeft)

end