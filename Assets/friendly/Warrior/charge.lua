function setup()
    AbilitySetCastType(Bor(ABILITY_TARGET_POINT,ABILITY_TARGET_ENEMY)); 
    AbilitySetPortrait("Assets/friendly/Warrior/icon_charge.png");
    

    SetAbilityRange(60)
    SetAbilityCooldown(0);

end

local aoe = -1;

function casted(x,y,obj,headingx,headingy)

    
    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    --f1["numTriggers"] = 5
    f1["value"] = 20;  
    --f1["duration"] = 10;
    f1["triggersPerSecond"] = 1


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
    SetMovePoint(GetMouseX(),GetMouseY())
    CreateCircularProjectiles(xPos,yPos,"",ATTACK_PROJECTILE_ANGLE,sp,duration,true,ATTACK_HITS_ENEMIES,10,{f1})

    --aoe = CreateAOE(GetX(obj),GetY(obj),"", 30, 1, 10, false, ATTACK_HITS_FRIENDLIES, {f1})

    return true; 
end

function untoggle()
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft, obj, target)
    Print("g: " .. g);

    MoveAttack(GetX(obj),GetY(obj));

end