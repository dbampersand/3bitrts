local xMin = 60
local xMax = 220

local yMin = 38
local yMax = 220



function setup()
    SetAbilityRange(20)
    SetCooldown(40);
    AbilitySetPortrait("assets/enemies/cusith/ability_focus.png");
    SetDescription("[b]Fan of Knives\n\nThrows knives, dealing large area damage.")
    
end

function casted(x,y,obj,headingx,headingy)


    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 30;  

    CreateCircularProjectiles(GetX(GetObjRef()),GetY(GetObjRef()),"",ATTACK_PROJECTILE_ANGLE,80,999,false,ATTACK_HITS_ENEMIES,128,COLOR_DAMAGE,0,{f1})

    return true;
end

function onhit(x,y,objhit)

end
function ontimeout(x,y,obj,dt,target, attack)
end

function abilitytick(x, y, timeUntilAttackTick, parent, target, dt, attackRef, totalAttackDurationLeft)

end