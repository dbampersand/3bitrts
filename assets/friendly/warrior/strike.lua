local speed = 50;
local duration = 2;


function setup()
    AbilitySetPortrait("assets/friendly/warrior/strike.png");
    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityRange(40)
    SetDescription("Strike\n\nStrikes the target, dealing damage and adding high amounts of threat. Deals more damage when Shield is on cooldown.")
    SetCooldown(6)
end
function casted(x,y,obj,headingx,headingy)
    local damage = 60;

    if (GetAbilityCooldown(GetObjRef(),1) > 0) then
        damage = 150
    end

    local f2 = {};
    f2["trigger"] = TRIGGER_INSTANT
    f2["type"] = EFFECT_HURT
    f2["value"] = damage;  

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_THREAT;
    f1["triggersPerSecond"] = 1 
    f1["value"] = 100;  
    f1["duration"] = 1;  

    local f3 = {};
    f3["trigger"] = TRIGGER_CONST;
    f3["type"] = EFFECT_SPEED;
    f3["value"] = 100;  
    f3["duration"] = 1; 

    ApplyEffect(obj,{f1,f2})
    ApplyEffect(GetObjRef(),{f3});
    PlaySound("assets/friendly/warrior/audio/strike.wav",0.5);

    PushObj(GetX(GetObjRef()),GetY(GetObjRef()),obj,50,0.15);
    ShakeScreen(5,0.25)
    
    return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end