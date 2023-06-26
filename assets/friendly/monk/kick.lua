
function setup()
    AbilitySetPortrait("assets/friendly/monk/icon_kick.png");
    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityRange(50)
    SetDescription("Kick\n\nStuns the target. Generates a chi.")
    SetCooldown(20);
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/monk/audio/kick.wav",1)
    
    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT
    f1["type"] = EFFECT_STUN
    f1["value"] = 2

    ApplyEffect(obj,{f1})

    AddStack(GetObjRef(),2,1);
    AddStack(GetObjRef(),3,1);
    AddStack(GetObjRef(),4,1);

    ShakeScreen(3,0.1)

    return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end