
local length = 100
local radius = 30

local hps = 60
local duration = 10
local tickRate = 0.75

function setup()
    AbilitySetCastType(ABILITY_POINT); 
    AbilitySetPortrait("assets/friendly/chromamancer/icon_infrared.png");
    SetDescription("Infrared\n\nProjects a cone of healing infrared light.")
    SetAbilityRange(length)
    SetAbilityHint(HINT_CONE,radius,false,length)
    SetCooldown(40);

end

function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/chromamancer/audio/infrared.wav",1)
   
    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT
    f1["type"] = EFFECT_HEAL
    f1["value"] = hps * tickRate

    local cone = CreateCone(GetX(GetObjRef()),GetY(GetObjRef()),x,y,"", radius, tickRate, duration, false, ATTACK_HITS_FRIENDLIES, COLOR_HEAL, DITHER_HORIZONTAL_QUARTER,length, {f1})
    return true; 
end

function untoggle()
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft, obj, target)
end
function applyattack(a,x,y)
    PlaySound("assets/friendly/chromamancer/audio/infrared_tick.wav",0.5,x,y);
end