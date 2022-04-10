local speed = 50;
local duration = 2;


function setup()
    AbilitySetPortrait("Assets/friendly/Warrior/strike.png");
    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityRange(40)
    SetDescription("Strike\n\nStrikes the target, dealing damage and adding high amounts of threat.")
    
end
function casted(x,y,obj,headingx,headingy)
    f2 = {};
    f2["trigger"] = TRIGGER_INSTANT
    f2["type"] = EFFECT_HURT
    f2["value"] = 10;  

    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_THREAT;
    f1["triggersPerSecond"] = 1 
    f1["value"] = 100;  
    f1["duration"] = 1;  

    f3 = {};
    f3["trigger"] = TRIGGER_CONST;
    f3["type"] = EFFECT_SPEED;
    f3["value"] = 100;  
    f3["duration"] = 5; 

    ApplyEffect(obj,{f1,f2})
    ApplyEffect(GetThisObj(),{f3});
    return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end