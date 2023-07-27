local heal = 80

local numToMake = 10
local timeBetween = 0.1

local speed = 30
local numProjectiles = 12
local radius = 70

function setup()

    SetAbilityRange(999)
    AbilitySetCastType(ABILITY_TARGET_FRIENDLY);
    SetAbilityHint(HINT_LINE,80);
    SetCooldown(12); 
    AbilitySetPortrait("assets/enemies/viper/ability_bite.png");
    SetDescription("[b]Splash\n\nSplashes a target with a wave of healing waters.")
end
function casted(x,y,obj,headingx,headingy)
    local f1 = {}   
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HEAL;
    f1["value"] = heal;

    CreateConeProjectiles(GetX(GetObjRef()),GetY(GetObjRef()),GetX(obj),GetY(obj),"",ATTACK_PROJECTILE_ANGLE,speed,999,false,ATTACK_HITS_FRIENDLIES,numProjectiles,COLOR_HEAL,radius,{f1})
    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft,parent,target,dt,attackRef)
end

function applyattack(atk,x,y)
end
function timerbeforetick(t,func)

    if (func == CreateAOE) then
        UpdateTimerArgument(t,1,GetX(GetObjRef()))
        UpdateTimerArgument(t,2,GetY(GetObjRef()))
    end
end