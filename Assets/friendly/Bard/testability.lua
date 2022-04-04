local speed = 50;
local duration = 2;


function setup()
    AbilitySetPortrait("Assets/friendly/Bard/icon_shield.png");
    AbilitySetCastType(ABILITY_POINT);
    SetAbilityRange(duration * speed)
    
end
function casted()
    f1 = {};
    f1["trigger"] = TRIGGER_TIMER;
    f1["type"] = EFFECT_HEAL;
    f1["triggersPerSecond"] = 2
    f1["value"] = 2;  
    f1["duration"] = 10;

    f2 = {};
    f2["trigger"] = TRIGGER_INSTANT
    f2["type"] = EFFECT_DAMAGE
    f2["triggersPerSecond"] = 1 
    f2["value"] = 10; 

    CreateProjectile(GetMouseX(),GetMouseY(),"",ATTACK_PROJECTILE_ANGLE,speed,duration,true,ATTACK_HITS_ENEMIES,{f1,f2})
    return true;
end

function onhit(x,y,objhit)
    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_DAMAGE;
    f1["triggersPerSecond"] = 1
    f1["duration"] = 1
    f1["value"] = 20;  

    --CreateAOE(x,y,"", 30, 1, 10,false, {f1})
end

function abilitytick(x, y, durationLeft)
    
end