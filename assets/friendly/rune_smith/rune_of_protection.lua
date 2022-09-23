local speed = 50;
local duration = 15;



function setup()
    AbilitySetPortrait("assets/friendly/paladin/icon_circle_of_protection.png");
    AbilitySetCastType(ABILITY_INSTANT);
    SetAbilityRange(40)
    SetDescription("Rune of Protection\n\nShields you from damage, applying a large amount of armor.")
    SetCooldown(duration)
end
function casted(x,y,obj,headingx,headingy)


    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_SHIELD;
    f1["value"] = 50;  
    f1["duration"] = 8;  


    f2 = {};
    f2["trigger"] = TRIGGER_INSTANT;
    f2["type"] = EFFECT_ARMOR;
    f2["value"] = 6;  
    f2["duration"] = 8;
    
    ApplyEffect(GetObjRef(),{f1,f2});


    PlaySound("assets/friendly/warrior/audio/strike.wav",0.5);
    
    return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end