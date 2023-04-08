local duration = 0.1

local dps = 40
local ticksPerSec = 2

local angle = 60
local numAoEs = 8

local radius = 15
local duration = 10

function setup()

    SetAbilityRange(999)
    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityHint(HINT_CIRCLE,30,false);
    SetCooldown(16); 
    AbilitySetPortrait("assets/enemies/viper/ability_bite.png");
    SetDescription("[b]Spit Fire\n\nSpits an arc of fire.")
end
function casted(x,y,obj,headingx,headingy)

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = dps / ticksPerSec;  
    local point = {};
    point.x = GetX(obj)
    point.y = GetY(obj)
    point = RotatePoint(point.x,point.y,GetX(GetObjRef()),GetY(GetObjRef()),-angle/2);
    for i = 0, numAoEs do
        local atk = CreateAOE(point.x,point.y,"", radius,  1/ticksPerSec,duration, false, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_HORIZONTAL_QUARTER, false, obj, {f1})
        point = RotatePoint(point.x,point.y,GetX(GetObjRef()),GetY(GetObjRef()),angle/numAoEs);
        SetAttackInactive(atk,(i+1)/2)

    end

    PlaySound("assets/enemies/rock_tosser/audio/chuck.wav",1);


    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)

end

function ontimeout(x,y,obj,dt,target)
end
