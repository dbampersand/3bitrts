
local length = 100
local radius = 30

local dps = 25
local duration = 3
local tickRate = 0.75

local cdReduce = 3

function setup()
    AbilitySetCastType(ABILITY_POINT); 
    AbilitySetPortrait("assets/friendly/chromamancer/icon_illusion.png");
    SetDescription("Ultraviolet\n\nProjects a cone of damaging ultraviolet light. Damage ticks reduce the cooldown of Infrared.")
    SetAbilityRange(length)
    SetAbilityHint(HINT_CONE,radius,false,length)
    SetCooldown(20);

end

function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/chromamancer/audio/ultraviolet.wav",1)
   
    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT
    f1["type"] = EFFECT_HURT
    f1["value"] = dps * tickRate

    local cone = CreateCone(GetX(GetObjRef()),GetY(GetObjRef()),x,y,"", radius, tickRate, duration, true, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_HORIZONTAL_QUARTER,length, {f1})
    return true; 
end

function untoggle()
end

function onhit(x,y,objhit)
    ReduceCooldown(GetObjRef(),1,cdReduce);
end

function abilitytick(x, y, durationLeft, obj, target)
end
function applyattack(a,x,y)
    PlaySound("assets/friendly/chromamancer/audio/ultraviolet_tick.wav",0.5,x,y);
end