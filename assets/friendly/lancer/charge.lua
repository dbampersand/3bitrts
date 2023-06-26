
local dps = 250

local duration = 0.45   
local tickRate = 1/30

local coneSize = 80
local radius = 60

local angle = {}


function setup()
    AbilitySetCastType(Bor(ABILITY_POINT,ABILITY_TARGET_ENEMY)); 
    AbilitySetPortrait("assets/friendly/lancer/icon_charge.png");
    SetAbilityHint(HINT_CONE,radius,false,coneSize)

    SetAbilityRange(coneSize)
    SetCooldown(30);
    SetDescription("Charge\n\nCharges an area.")

end

function casted(x,y,obj,headingx,headingy)

    PlaySound("assets/friendly/lancer/audio/charge.wav",0.5);
    
    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = dps * tickRate;  

    local f2 = {};
    f2["trigger"] = TRIGGER_CONST;
    f2["type"] = EFFECT_SPEED;
    f2["duration"] = duration
    f2["value"] = 250;  

    ApplyEffect(GetObjRef(),{f2})

    angle = GetHeadingVector(GetX(GetObjRef()),GetY(GetObjRef()),x,y)

    SetMovePoint(GetObjRef(),GetX(GetObjRef())+(angle.headingx*1000), GetY(GetObjRef())+(angle.headingy*1000), true, false)
    
    SetObjectPush(GetObjRef(),true)
    BlockCommands(GetObjRef(),true)

    After(SetObjectPush,duration,false,false)
    After(BlockCommands,duration,false,GetObjRef(),false)
    After(StopCommand,duration,false,GetObjRef())

    ShakeScreen(1,duration)



    local cone = CreateCone(GetX(GetObjRef()),GetY(GetObjRef()),x,y,"", radius, tickRate, duration, false, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_HORIZONTAL_QUARTER, coneSize, {f1})
    return true; 
end

function untoggle()
end

function timerbeforetick(timer) 
end

function onhit(x,y,objHit)
end

function abilitytick(x, y, durationLeft, obj, target, dt, attackRef)
    SetAttackPosition(attackRef,GetX(GetObjRef()),GetY(GetObjRef()))
    SetAttackTargetPosition(attackRef,GetX(GetObjRef())+angle.headingx,GetY(GetObjRef())+angle.headingy)
end