local duration = 8
local attackSpeedReduction = -0.2
local cooldownReduction = 0.2

local dps = 25

function setup()
    AbilitySetPortrait("assets/friendly/shaman/icon_embody.png");
    SetDescription("Embody\n\nPoisons the target and increases their attack speed and reduces their cooldowns.")
    AbilitySetHintColor(EFFECT_HURT)

    AbilitySetCastType(ABILITY_TARGET_ALL);

    SetCooldown(12);
end


function casted(x,y,obj,headingx,headingy)
    local f1 = {};
    f1["trigger"] = TRIGGER_CONST;
    f1["type"] = EFFECT_ATTACKSPEED;
    f1["value"] = attackSpeedReduction;
    f1["duration"] = duration;
    f1["name"] = "Embody"
    f1["overwrites"] = true

    local f1 = {};
    f1["trigger"] = TRIGGER_CONST;
    f1["type"] = EFFECT_COOLDOWN_RATE
    f1["value"] = cooldownReduction;
    f1["duration"] = duration;
    f1["name"] = "Embody Cooldown"
    f1["overwrites"] = true


    local f2 = {};
    f2["trigger"] = TRIGGER_TIMER;
    f2["type"] = EFFECT_HURT;
    f2["value"] = dps;
    f2["duration"] = duration;
    f2["name"] = "Embody Poison"
    f2["overwrites"] = true

    ApplyEffect(obj, {f1,f2})

    PlaySound("assets/friendly/shaman/audio/embody.wav",1);

    return true; 
end

function untoggle()
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft, obj, target)
end