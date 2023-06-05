local numCasts = 0
local duration = 32

local radius = 30

local angle = 30

local dps = 160
local ticksPerSec = 0.25


function setup()
    SetAbilityRange(256) 
    SetCooldown(1);

    AbilitySetCastType(ABILITY_POINT);
    AbilitySetPortrait("assets/enemies/kobold_miner/ability_throw_bomb.png");
    SetDescription("[b]Hellfire\n\nDeals damage in a moving area.");
    SetAbilityName("Hellfire"); 

end

function casted(x,y,obj,headingx,headingy)
    local delta = 0


    if (math.fmod(numCasts,2) == 0) then
        delta = angle
    else
        delta = -angle
    end

    local point = {}
    point.x = (GetX(GetObjRef()) - x)
    point.y = (GetY(GetObjRef()) - y)
    point = Normalize(point.x,point.y);
    point.x = GetX(GetObjRef()) + (point.x * 20);
    point.y = GetY(GetObjRef()) + (point.y * 20);

    point = RotatePoint(point.x,point.y,GetX(GetObjRef()), GetY(GetObjRef()), delta)


    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = dps * ticksPerSec;

    local aoe = CreateAOE(point.x,point.y,"", radius, ticksPerSec, 20, false, ATTACK_HITS_ENEMIES,COLOR_DAMAGE,DITHER_DAMAGE_EIGTH,false, -1, {f1})

    SetAttackMoveAngle(aoe,x-point.x,y-point.y)
    SetAttackVelocity(aoe,80)

    numCasts = numCasts + 1
    return true; 
end

function onhit(x,y,objhit)
end
function ontimeout(x,y,obj,dt,target)
end
function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
    SetAttackVelocity(attackRef,GetAttackVelocity(attackRef)+dt*10)

end
function onchanneled() 
end