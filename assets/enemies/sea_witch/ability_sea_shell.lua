local tickrate = 0.5
local duration = 12
local dps = 30

local radius = 35
local coneSize = 70

local numCones = 8
local changeDirectionTime = 1

local duration = 1

function setup()

    SetAbilityRange(256)
    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityHint(HINT_LINE,80);
    SetCooldown(12); 
    AbilitySetPortrait("assets/enemies/viper/ability_bite.png");
    SetDescription("[b]Sea Shell\n\nCreates several damaging cones.")
end
function casted(x,y,obj,headingx,headingy)
    local f1 = {}
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = dps * tickrate;

    local heading = GetHeadingVector(GetX(GetObjRef()),GetY(GetObjRef()),GetX(obj),GetY(obj));
    local startAngle = math.atan2(heading.headingx,heading.headingy)
    for i = 1, numCones do
        local angle = DegToRad(180 / numCones * (i))   

        angle = angle - startAngle;
        local xTo = math.cos(angle)*10000
        local yTo = math.sin(angle)*10000

        cone = CreateCone(GetX(GetObjRef()),GetY(GetObjRef()),GetX(GetObjRef())+xTo,GetY(GetObjRef())+yTo,"", radius, duration, duration, true, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_HORIZONTAL_QUARTER,coneSize, {f1})

    end
    return true; 
end

function onhit(x,y,objhit)
end
function ontimeout(x,y,parent,dt,target,attack)
end
function abilitytick(x, y, durationLeft,parent,target,dt,attackRef)
    SetAttackPosition(attackRef,GetX(GetObjRef()),GetY(GetObjRef()))
end

function applyattack(atk,x,y)
end
function timerbeforetick(t,func)
end