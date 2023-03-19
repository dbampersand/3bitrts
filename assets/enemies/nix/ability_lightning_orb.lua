local aoe = -1
local direction = {};
local speed = 60;
local defaultSpeed = 30;

local triggersPerSec = 6
local dps = 25
function setup()
    SetAbilityRange(32) 
    SetCooldown(4);
    AbilitySetPortrait("assets/enemies/kobold_miner/ability_throw_bomb.png");
    SetDescription("[b]Lightning Orb\n\nShoots a ball that moves, dealing damage to all in its path.");
    SetAbilityName("Lightning Orb"); 

end

function casted(x,y,obj,headingx,headingy)
    
    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = dps / triggersPerSec;

    aoe = CreateAOE(x,y,"", 20, 1 / triggersPerSec, 5, false, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_DAMAGE_QUARTER, false, -1, {f1})
    direction.x = GetX(obj) - GetX(GetObjRef()) 
    direction.y = GetY(obj) - GetY(GetObjRef()) 
    direction = Normalize(direction.x,direction.y);
    speed = defaultSpeed;
    return true; 
end

function onhit(x,y,objhit)
end
function ontimeout(x,y,obj,dt,target)
    aoe = -1;
end
function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
    if (attackRef >= 0) then

        x = x + (direction.x * speed) * dt;
        y = y + (direction.y * speed) * dt;
        SetAttackPosition(attackRef,x,y)
        speed = speed + dt*2;
    end
end
function onchanneled() 
end