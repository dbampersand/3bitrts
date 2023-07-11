local duration = 15
local value = -6
local damage = 100
function setup()
    AbilitySetPortrait("assets/friendly/herbalist/icon_dissolve.png");
    SetDescription("Dissolve\n\nReduces the target's armour and deals damage over time.")

    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    AbilitySetHintColor(EFFECT_ARMOR)


    SetCooldown(20);
    SetManaCost(10)

    
end


function casted(x,y,obj,headingx,headingy)
    local f1 = {};
    f1["trigger"] = TRIGGER_CONST
    f1["type"] = EFFECT_ARMOR
    f1["duration"] = duration;
    f1["value"] = value;

    local f2 = {};
    f2["trigger"] = TRIGGER_TIMER
    f2["type"] = EFFECT_HURT
    f2["duration"] = duration;
    f2["value"] = damage / duration;

    ApplyEffect(obj,{f1,f2});   

    PlaySound("assets/friendly/herbalist/audio/dissolve.wav",1.25)


    return true; 
end

function untoggle()
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft, obj, target)
end