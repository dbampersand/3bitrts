local speed = 50;
local duration = 2;


function setup()
    AbilitySetPortrait("assets/friendly/chromamancer/icon_saturate.png");
    AbilitySetCastType(ABILITY_TARGET_FRIENDLY);
    SetAbilityRange(40)
    SetDescription("Saturate\n\nHeals a massive amount and applies a shield to you and your target.")
    SetCooldown(40);
    
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/chromamancer/audio/saturate.wav",1)


    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_SHIELD;
    f1["value"] = 400;  
    f1["duration"] = 15;

    f2 = {};
    f2["trigger"] = TRIGGER_INSTANT;
    f2["type"] = EFFECT_HEAL;
    f2["value"] = 10000;  


    ApplyEffect(obj,{f1,f2});
    ApplyEffect(GetObjRef(),{f1});

    return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end