
function setup()
    AbilitySetPortrait("assets/friendly/rogue/icon_stab.png");
    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityRange(50)
    SetDescription("Stab\n\nStabs the target and reduces the cooldown of Deceptive Strike by 2 seconds.")
    SetCooldown(4)
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/rogue/audio/stab.wav",1)

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT
    f1["type"] = EFFECT_HURT
    f1["value"] = 50;  

    ApplyEffect(obj,{f1})

    ReduceCooldown(GetObjRef(),1,2);

    return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end