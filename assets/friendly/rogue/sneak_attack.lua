
function setup()
    AbilitySetPortrait("assets/friendly/rogue/icon_shiv.png");
    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityRange(50)
    SetDescription("Stab\n\nStabs the target and reduces the cooldown of Deceptive Strike by 2 seconds.")
    SetCooldown(8)
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/rogue/audio/shiv.wav",1.25)

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT
    f1["type"] = EFFECT_HURT
    f1["value"] = 100;  

    ApplyEffect(obj,{f1})
    return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end