
local damage = 400

local duration = 0.45

local coneSize = 90
local radius = 20

local angle = {}

function setup()
    AbilitySetCastType(Bor(ABILITY_POINT,ABILITY_TARGET_ENEMY)); 
    AbilitySetPortrait("assets/friendly/lancer/icon_lance.png");
    SetAbilityHint(HINT_CONE,radius,false,coneSize)
    AbilitySetHintColor(EFFECT_HURT)

    SetAbilityRange(coneSize)
    SetCooldown(10);
    SetDescription("Lance\n\nSmashess targets in a cone and pushes them away.")

end

local aoe = -1;

function casted(x,y,obj,headingx,headingy)

    PlaySound("assets/friendly/lancer/audio/lance.wav",0.5);
    
    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = damage;  


    local cone = CreateCone(GetX(GetObjRef()),GetY(GetObjRef()),x,y,"", radius, 0, 0, false, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_HORIZONTAL_QUARTER, coneSize, {f1})

    local objsHit = GetObjectsInCone(TYPE_ENEMY,GetX(GetObjRef()),GetY(GetObjRef()),x,y,radius,coneSize)
    for i = 1, #objsHit do
        PushObj(GetX(GetObjRef()),GetY(GetObjRef()),objsHit[i],100,0.1)
    end
    ShakeScreen(#objsHit,0.2)
    return true; 
end

function untoggle()
end

function timerbeforetick(timer) 
end

function onhit(x,y,objHit)
end

function abilitytick(x, y, durationLeft, obj, target, dt, attackRef)
end