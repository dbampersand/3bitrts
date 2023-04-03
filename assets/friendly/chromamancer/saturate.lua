 local speed = 50;
local duration = 2;


function setup()
    AbilitySetPortrait("assets/friendly/chromamancer/icon_saturate.png");
    AbilitySetCastType(ABILITY_TARGET_FRIENDLY);
    SetAbilityRange(40)
    SetDescription("Saturate\n\nHeals a target fully and applies a shield to you and your target for the amount healed.")
    SetCooldown(40);
    
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/chromamancer/audio/saturate.wav",1)

    local healFor = GetMaxHP(obj) - GetHP(obj);
    if (healFor < 0) then
        healFor = 0;
    end


    local f2 = {};
    f2["trigger"] = TRIGGER_INSTANT;
    f2["type"] = EFFECT_HEAL;
    f2["value"] = healFor;  

    ApplyEffect(obj,{f2});


    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_SHIELD;
    f1["value"] = healFor;  
    f1["duration"] = 15;

    ApplyEffect(obj,{f1});
    ApplyEffect(GetObjRef(),{f1});

    return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end