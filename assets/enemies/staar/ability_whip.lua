local tickrate = 0.5
local duration = 12
local dps = 30

local radius = 30
local lengthMax = 80
local numCones = 8

function setup()

    SetAbilityRange(256)
    AbilitySetCastType(ABILITY_INSTANT);
    SetAbilityHint(HINT_LINE,80);
    SetCooldown(16); 
    AbilitySetPortrait("assets/enemies/viper/ability_bite.png");
    SetDescription("[b]Whip\n\nThrashes several tentacles.")
end
function casted(x,y,obj,headingx,headingy)
    local f1 = {}
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = dps * tickrate;

    for i = 1, numCones do
        local length = RandRange(30,lengthMax)
        local angle = 360 / numCones    

        local x2 = cos(angle) * 10000
        local y2 = sin(angle) * 10000


        CreateCone(GetX(GetObjRef()),GetY(GetObjRef()),GetX(GetObjRef())+x2,GetY(GetObjRef())+y2,"",radius,tickrate,duration,true,ATTACK_HITS_ENEMIES,COLOR_DAMAGE,DITHER_NONE,length,{f1})
    end
    return true; 
end

function onhit(x,y,objhit)
end
function ontimeout(x,y,parent,dt,target,attack)
end
function abilitytick(x, y, durationLeft,parent,target,dt,attackRef)

end

function applyattack(atk,x,y)
end
function timerbeforetick(t,func)
end