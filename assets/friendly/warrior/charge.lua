
local damage = 40
function setup()
    AbilitySetCastType(Bor(ABILITY_POINT,ABILITY_TARGET_ENEMY)); 
    AbilitySetPortrait("assets/friendly/warrior/icon_charge.png");
    

    SetAbilityRange(60)
    SetCooldown(20);
    SetDescription("Charge\n\nCharges the target, dealing " .. damage  .. "damage to targets around you.")

end

local aoe = -1;

function casted(x,y,obj,headingx,headingy)

    PlaySound("assets/friendly/warrior/audio/charge.wav",0.5);
    
    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    --f1["numTriggers"] = 5
    f1["value"] = damage;  
    --f1["duration"] = 10;
    
    f2 = {};
    f2["trigger"] = TRIGGER_INSTANT;
    f2["type"] = EFFECT_THREAT;
    f2["value"] = 100;  



   -- Teleport(GetObjRef(),GetMouseX(),GetMouseY());
    xPos = GetX();
    yPos = GetY();
    local sp = 50 
    local duration = 5
    speed = {};
    speed["trigger"] = TRIGGER_CONST;
    speed["type"] = EFFECT_SPEED; 
    speed["value"] = 400; 
    speed["duration"] = 0.1
    ApplyEffect(GetObjRef(),{speed});

    SetMovePoint(GetObjRef(),x, y, true, false)
    CreateCircularProjectiles(xPos,yPos,"",ATTACK_PROJECTILE_ANGLE,sp,duration,true,ATTACK_HITS_ENEMIES,10,COLOR_FRIENDLY_DAMAGE,{f1,f2})

    --aoe = CreateAOE(GetX(obj),GetY(obj),"", 30, 1, 10, false, ATTACK_HITS_FRIENDLIES,false,-1, {f1})

    return true; 
end

function untoggle()
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft, obj, target)

    --MoveAttack(GetAttackRef(),GetX(obj),GetY(obj));

end