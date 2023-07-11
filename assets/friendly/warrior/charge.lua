
local damage = 70
local armorDuration = 20
function setup()
    AbilitySetCastType(Bor(ABILITY_POINT,ABILITY_TARGET_ENEMY)); 
    AbilitySetPortrait("assets/friendly/warrior/icon_charge.png");
    SetAbilityHint(HINT_LINE,1);
    AbilitySetHintColor(EFFECT_HURT)

    SetAbilityRange(60)
    SetCooldown(20);
    SetDescription("Charge\n\nCharges the target, dealing " .. damage  .. "damage to targets around you. Increases armor for " .. armorDuration .. " seconds.")

end

local aoe = -1;

function casted(x,y,obj,headingx,headingy)

    PlaySound("assets/friendly/warrior/audio/charge.wav",0.5);
    
    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    --f1["numTriggers"] = 5
    f1["value"] = damage;  
    --f1["duration"] = 10;
    
    local f2 = {};
    f2["trigger"] = TRIGGER_INSTANT;
    f2["type"] = EFFECT_THREAT;
    f2["value"] = 600; 



   -- Teleport(GetObjRef(),GetMouseX(),GetMouseY());
    xPos = GetX(GetObjRef());
    yPos = GetY(GetObjRef());
    local sp = 50 
    local duration = 2.25
    local speed = {};
    speed["trigger"] = TRIGGER_CONST;
    speed["type"] = EFFECT_SPEED; 
    speed["value"] = 400; 
    speed["duration"] = 0.1

    local armor = {};
    armor["trigger"] = TRIGGER_CONST;
    armor["type"] = EFFECT_ARMOR; 
    armor["value"] = 10; 
    armor["duration"] = armorDuration


    ApplyEffect(GetObjRef(),{speed,armor});

    SetMovePoint(GetObjRef(),x, y, true, false)
    After(CreateCircularProjectiles,0.08,false,x,y,"",ATTACK_PROJECTILE_ANGLE,sp,duration,true,ATTACK_HITS_ENEMIES,12,COLOR_FRIENDLY_DAMAGE,0,{f1,f2});
    return true; 
end

function untoggle()
end

function timerbeforetick(timer) 
    UpdateTimerArgument(timer,1,GetX(GetObjRef()))
    UpdateTimerArgument(timer,2,GetY(GetObjRef()))
end

function onhit(x,y,objHit)
    PushObj(x,y,objHit,90,0.1);
    ShakeScreen(0.5,0.1)

    PlaySound("assets/friendly/warrior/audio/charge_hitsound.wav",0.5);


end

function abilitytick(x, y, durationLeft, obj, target)

    --MoveAttack(GetAttackRef(),GetX(obj),GetY(obj));

end