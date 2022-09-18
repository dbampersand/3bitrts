
function setup()
    AbilitySetPortrait("assets/friendly/ranger/icon_cripple.png");
    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityRange(50)
    SetDescription("Kick\n\nStuns the target. Generates a chi.")
    SetCooldown(20);
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/rogue/audio/backstab.wav",1.25)
    
    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT
    f1["type"] = EFFECT_STUN
    f1["value"] = 2

    ApplyEffect(obj,{f1})

    AddStack(GetObjRef(),2,1);
    AddStack(GetObjRef(),3,1);
    AddStack(GetObjRef(),4,1);

    return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end