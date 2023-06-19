local duration = 12

function setup()
    SetAbilityRange(200)
    AbilitySetCastType(ABILITY_INSTANT);
    SetAbilityHint(HINT_LINE,80);
    SetCooldown(20); 
    AbilitySetPortrait("assets/enemies/skeleton_minion_melee/ability_grasp.png");
    SetDescription("[b]Grasp\n\nReduces the target's ability cooldown speed.")
end
function casted(x,y,obj,headingx,headingy)

    local f1 = {};
    f1["trigger"] = TRIGGER_CONST;
    f1["type"] = EFFECT_COOLDOWN_RATE;
    f1["value"] = -0.15;
    f1["duration"] = duration
    f1["stacks"] = false

    ApplyEffect(obj,{f1})

    PlaySound("assets/enemies/skeleton_minion_melee/audio/grasp.wav",0.5);
    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
end