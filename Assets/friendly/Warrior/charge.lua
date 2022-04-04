function setup()
    AbilitySetCastType(ABILITY_TARGET_POINT);
    AbilitySetPortrait("Assets/friendly/Warrior/icon_charge.png");


    SetAbilityRange(60)
    SetAbilityCooldown(0);

end

local aoe = -1;

function casted(x,y,obj,headingx,headingy)

    
    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_DAMAGE;
    --f1["numTriggers"] = 5
    f1["value"] = 20;  
    --f1["duration"] = 10;
    f1["triggersPerSecond"] = 1


    Teleport(GetObjRef(),GetMouseX(),GetMouseY());
    xPos = GetX();
    yPos = GetY();
    local speed = 50 
    local duration = 5
    for i = 1,10 do
        --CreateCircularProjectiles(xPos,yPos,speed,duration,ATTACK_HITS_ENEMIES,{f1},10);
        --CreateProjectile(xPos,yPos,"",ATTACK_PROJECTILE_ANGLE,speed,duration,true,ATTACK_HITS_ENEMIES,{f1})

        --CreateProjectile()
    end 
    
    --aoe = CreateAOE(GetX(obj),GetY(obj),"", 30, 1, 10, false, ATTACK_HITS_FRIENDLIES, {f1})

    return true; 
end

function untoggle()
    Print(aoe);
    RemoveAttack(aoe);
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft, obj, target)
    Print("g: " .. g);

    MoveAttack(GetX(obj),GetY(obj));

end