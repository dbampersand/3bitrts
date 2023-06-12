local duration = 0.1

local dps = 20
local ticksPerSec = 2

local angle = 70
local numAoEs = 9

local radius = 20
local duration = 12


function setup()
    SetAbilityRange(256)
    SetCooldown(3.5);
    AbilitySetPortrait("assets/enemies/wraith/ability_throw_darts.png");
    SetDescription("[b]Desolate\n\nThrows an arc of desolation around the target.")
    SetAbilityName("Desolate"); 
    SetAbilityHint(HINT_CIRCLE,20);

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
        SetAttackInactive(atk,(i+1)/4)

    end

    PlaySound("assets/enemies/rock_tosser/audio/chuck.wav",1);



    return true; 
end

function onhit(x,y,objhit)

end
function ontimeout(x,y,obj,dt,target)

end
function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
    
end
function onchanneled() 
    SetChannelingSprite("assets/enemies/wraith/wraith_casting_desolate.png");
end