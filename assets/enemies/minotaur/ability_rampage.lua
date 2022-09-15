local delay = 5
local radius = 60

local length = 10
local timer = 0
local shouldChange = true
function setup()
    SetAbilityRange(256)
    SetCooldown(length);
    AbilitySetPortrait("assets/enemies/minotaur/ability_rampage.png");
    SetDescription("[b]Rampage\n\nMoves wildly, causing damage around it.")

end

local atk = -1
local attackActive = false;

function casted(x,y,obj,headingx,headingy)
    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 10;  

    local speed = {}
    speed["trigger"] = TRIGGER_CONST
    speed["type"] = EFFECT_SPEED
    speed["duration"] = 10
    speed["value"] = 100

    atk = CreateAOE(GetX(GetObjRef()),GetY(GetObjRef()),"", radius, 0.1, length, true, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_NONE, false, GetObjRef(), {f1})
    ApplyEffect(GetObjRef(),{speed});
    timer = 0
    shouldChange = true

    return true; 
end

function onhit(x,y,objhit)

end
function ontimeout(x,y,obj,dt,target)
    EnableAI(GetObjRef(),true); 
    atk = -1

end
function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
    if (atk >= 0 and attackRef == atk) then
        x2 = GetX(target);
        y2 = GetY(target);
        MoveAttack(atk,x2,y2);
    end

    EnableAI(GetObjRef(),false);
    if (math.fmod((timer),0.6) <= 0.1) then
        if (shouldChange == true) then
            SetMovePoint(RandRange(0,255),RandRange(0,255),false,false);
        end
        shouldChange = false
    else
        shouldChange = true
    end
    timer = timer + dt;

    
end