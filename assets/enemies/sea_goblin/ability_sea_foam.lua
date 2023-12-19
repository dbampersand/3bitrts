local tickrate = 0.5
local duration = 12
local dps = 20

local radius = 10

local numAoEs = 4
local aoes = {}
local aoeTurn = 0.05

local distanceFrom = 30

local rotSpeed = 48

function setup()

    SetAbilityRange(256)
    AbilitySetCastType(ABILITY_INSTANT);
    SetAbilityHint(HINT_NONE,80);
    SetCooldown(16); 
    AbilitySetPortrait("assets/enemies/sea_goblin/ability_sea_foam.png");
    SetDescription("[b]Sea Foam\n\nCreates a series of waves that move in a circular motion.")
end
function casted(x,y,obj,headingx,headingy)

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = dps * tickrate;

    local toRotate = 360 / numAoEs;
    local rotation = 0
    x = GetX(GetObjRef())
    y = GetY(GetObjRef())

    for i = 1, numAoEs do 
        local point = RotatePoint(x,y-distanceFrom,GetX(GetObjRef()),GetY(GetObjRef()),rotation)
        local aoe = CreateAOE(point.x,point.y,"",radius,tickrate,duration,false,ATTACK_HITS_ENEMIES,COLOR_DAMAGE,DITHER_DAMAGE_HALF,false,-1,0,0,50,{f1});
        aoes[i] = aoe;
        rotation = rotation + toRotate;

    end

    PlaySound("assets/enemies/sea_goblin/audio/ability_seafoam.wav",1);
    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft,parent,target,dt,attackRef)
    local time = GetAttackLifetime(attackRef);

    local idx = 0
    for i = 1, numAoEs do 
        if (attackRef == aoes[i]) then
            idx = i
        end
    end

    local rotation = idx * (360/numAoEs)
    local pBefore = RotatePoint(GetX(GetObjRef()),GetY(GetObjRef())-distanceFrom,GetX(GetObjRef()),GetY(GetObjRef()),rotation)

    local newRotation = (1-(time/duration)) * 360 * dt * rotSpeed

    local point = RotatePoint(pBefore.x,pBefore.y,GetX(GetObjRef()), GetY(GetObjRef()),newRotation)
    SetAttackPosition(attackRef,point.x,point.y)
end

function applyattack(atk,x,y)
end