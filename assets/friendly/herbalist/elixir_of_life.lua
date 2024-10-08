local heal = 800
local duration = 5

function setup()
    AbilitySetPortrait("assets/friendly/herbalist/icon_elixir_of_life.png");
    AbilitySetCastType(ABILITY_TARGET_FRIENDLY);
    AbilitySetHintColor(EFFECT_HEAL)
    SetAbilityRange(60)
    SetDescription("Elixir of Life\n\nA large heal over time effect.")
    SetManaCost(8)
    SetAbilityMaxStacks(4);
    SetAbilityCooldown(GetObjRef(),GetAbilityRef(),20);
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/herbalist/audio/elixir_of_life.wav",1.25)

    local newObj = obj
    local newX = 0; local newY = 0


    local f1 = {};
    f1["trigger"] = TRIGGER_TIMER
    f1["type"] = EFFECT_HEAL
    f1["triggersPerSecond"] = 0.5
    f1["value"] = heal / duration * f1["triggersPerSecond"]; 
    f1["duration"] = duration;
    f1["name"] = "Elixir of Life"

    ApplyEffect(obj,{f1});
    return true;
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end