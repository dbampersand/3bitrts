local tickrate = 0.5
local duration = 4
local dps = 25

local radius = 15
local lengthMax = 80
local numCones = 6

local cones = {}

function setup()

    SetAbilityRange(256)
    AbilitySetCastType(ABILITY_INSTANT);
    SetAbilityHint(HINT_LINE,80);
    SetCooldown(16); 
    AbilitySetPortrait("assets/enemies/staar/ability_whip.png");
    SetDescription("[b]Whip\n\nThrashes several tentacles.")
end
function casted(x,y,obj,headingx,headingy)
    local f1 = {}
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = dps * tickrate;

    for i = 1, numCones do
        local length = RandRange(30,lengthMax)
        local angle = 2 * math.pi / numCones * (i)    

        local x2 = math.cos(angle)
        local y2 = math.sin(angle)
        cones[i] = {}
        cones[i].atk = CreateCone(GetX(GetObjRef()),GetY(GetObjRef()),GetX(GetObjRef())+x2,GetY(GetObjRef())+y2,"",radius,tickrate,duration,true,ATTACK_HITS_ENEMIES,COLOR_DAMAGE,DITHER_NONE,length,{f1})
        cones[i].toAngle = RandRange(0,2*math.pi)
        cones[i].angle = angle;
        cones[i].length = length;
        cones[i].changeDir = RandRange(0.1,0.75)
        cones[i].speed = RandRange(1,4)

    end
    return true; 
end

function onhit(x,y,objhit)
end
function ontimeout(x,y,parent,dt,target,attack)
end
function abilitytick(x, y, durationLeft,parent,target,dt,attackRef)
    for i = 1, #cones do
        if (attackRef == cones[i].atk) then
            cones[i].changeDir = cones[i].changeDir - dt;
            if (cones[i].changeDir < 0) then
                cones[i].changeDir = RandRange(0.1,0.75)
                cones[i].toAngle = RandRange(0,2*math.pi);
                cones[i].speed = RandRange(1,4)
            end
            cones[i].angle = Cerp(cones[i].angle,cones[i].toAngle,cones[i].speed*dt);
            SetAttackPosition(cones[i].atk,GetX(GetObjRef()),GetY(GetObjRef()))
            SetAttackTargetPosition(attackRef,GetX(GetObjRef()) + math.cos(cones[i].angle) * cones[i].length, GetY(GetObjRef()) + math.sin(cones[i].angle) * cones[i].length)
        end
    end
end
function ontimeout()
    for i = 1, #cones do
        RemoveAttack(cones[i])
    end
    cones = {}
end
function applyattack(atk,x,y)
end
function timerbeforetick(t,func)
end
function parentdeath(x, y, durationLeft, parent, target, attackRef)
    for i = 1, #cones do
        RemoveAttack(cones[i])
    end
    cones = {}
end