local duration = 8
local attackSpeedReduction = -0.2

function setup()
    AbilitySetPortrait("assets/friendly/shaman/icon_stoneskin_totem.png");
    SetDescription("Embody\n\nPoisons the target and increases their attack speed and reduces their cooldowns.")

    AbilitySetCastType(ABILITY_TARGET_ALL);

    SetCooldown(6);
end


function casted(x,y,obj,headingx,headingy)
    local f1 = {};
    f1["trigger"] = TRIGGER_CONST;
    f1["type"] = EFFECT_ATTACKSPEED;
    f1["value"] = attackSpeedReduction;
    f1["duration"] = duration;
    f1["name"] = "AttackSpeed"
    f1["overwrites"] = true

    local f2 = {};
    f2["trigger"] = TRIGGER_CONST;
    f2["type"] = EFFECT_ATTACKSPEED;
    f2["value"] = attackSpeedReduction;
    f2["duration"] = duration;
    f2["name"] = "AttackSpeed"
    f2["overwrites"] = true
    return true; 
end

function untoggle()
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft, obj, target)
end