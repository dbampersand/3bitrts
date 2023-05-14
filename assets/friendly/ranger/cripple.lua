local radius = 40
local length = 110
local push = 500  

function setup()
    AbilitySetPortrait("assets/friendly/ranger/icon_cripple.png");
    AbilitySetCastType(ABILITY_POINT);
    SetAbilityRange(length)
    SetAbilityHint(HINT_CONE,radius,false,length)
    SetDescription("Cripple\n\nPushes targets away and slows them.")
    SetCooldown(10);
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/ranger/audio/cripple.wav",1.25)
    local f1 = {};
    f1["trigger"] = TRIGGER_CONST
    f1["type"] = EFFECT_SPEED
    f1["value"] = -70;  
    f1["duration"] = 10

    local objs = GetObjectsInCone(TYPE_ENEMY,GetX(GetObjRef()),GetY(GetObjRef()),x,y,radius,length)
    for i = 1, #objs do
        Print("aaaa");
        local pushAmt = (1 - (GetDist(GetObjRef(),objs[i]) / length)) * push
        PushObj(GetX(GetObjRef()),GetY(GetObjRef()),objs[i],pushAmt,0.1)
    end

    local cone = CreateCone(GetX(GetObjRef()),GetY(GetObjRef()),x,y,"", radius, 0, 0, false, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_HORIZONTAL_QUARTER,length, {f1})
    return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end