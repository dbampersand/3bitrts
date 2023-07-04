local tickrate = 0.5
local duration = 12
local dps = 30

local radius = 35

local moveTime = 1

local target = -1

local accel = 10


function setup()

    SetAbilityRange(256)
    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityHint(HINT_LINE,80);
    SetCooldown(12); 
    AbilitySetPortrait("assets/enemies/viper/ability_bite.png");
    SetDescription("[b]Charge\n\nDashes at an enemy, creating a damaging wave.")
end
function casted(x,y,obj,headingx,headingy)
    EnableAI(GetObjRef(),false)

    SetMovePoint(GetObjRef(),GetX(obj),GetY(obj),true,true)
    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = dps * tickrate;

    target = obj

    local dir = GetHeadingVector(GetX(GetObjRef()),GetY(GetObjRef()),GetX(obj),GetY(obj))

    After(EnableAI,moveTime,false,GetObjRef(),true)
    After(CreateAOE,moveTime,false,GetX(GetObjRef()),GetY(GetObjRef()),"",radius,tickrate,999,true,ATTACK_HITS_ENEMIES,COLOR_DAMAGE,DITHER_DAMAGE_QUARTER,false,-1,dir.headingx,dir.headingy,30,{f1})
    PlaySound("assets/enemies/wolf/audio/call_pack.wav",0.35);
    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft,parent,target,dt,attackRef)
    AddAttackVelocity(attackRef,dt*accel)
end

function applyattack(atk,x,y)
end
function timerbeforetick(t,func)

    if (func == CreateAOE) then
        UpdateTimerArgument(t,1,GetX(GetObjRef()))
        UpdateTimerArgument(t,2,GetY(GetObjRef()))
        
        local dir = GetHeadingVector(GetX(GetObjRef()),GetY(GetObjRef()),GetX(target),GetY(target))
        
        --angle
        UpdateTimerArgument(t,13,dir.headingx)
        UpdateTimerArgument(t,14,dir.headingy)
        

    end
end