local duration = 15
local value = 10

function setup()
    AbilitySetPortrait("assets/friendly/herbalist/icon_dissolve.png");
    SetDescription("Dissolve\n\nReduces target's armor.")

    AbilitySetCastType(ABILITY_TARGET_ENEMY);

    SetCooldown(20);
    SetManaCost(10)
end


function casted(x,y,obj,headingx,headingy)
    local f1 = {};
    f1["trigger"] = TRIGGER_CONST
    f1["type"] = EFFECT_ARMOR
    f1["duration"] = duration;
    f1["value"] = value;


    ApplyEffect(obj,{f1});

    return true; 
end

function untoggle()
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft, obj, target)
end