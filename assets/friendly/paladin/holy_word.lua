function setup()
    AbilitySetPortrait("assets/friendly/paladin/icon_holy_word.png");
    AbilitySetCastType(ABILITY_INSTANT);
    SetDescription("Holy Word\n\nGenerates a shield.")
    AbilitySetHintColor(EFFECT_SHIELD)
    SetCooldown(0.5);
    SetManaCost(25)
end

function casted()
    PlaySound("assets/friendly/paladin/audio/holy_word.wav",1);

    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_SHIELD;
    f1["value"] = 250;  
    f1["duration"] = 15;  
    ApplyEffect(GetObjRef(),{f1});


    return true;
end

function onhit(x,y,objhit)
    objRef = GetObjRef();
    SetObjPosition(GetObjRef(),x-GetWidth(objRef)/2,y-GetHeight(objRef)/2);
    SetObjTargetPosition(GetObjRef(),x-GetWidth(objRef)/2,y-GetHeight(objRef)/2);
end

function abilitytick(x, y, durationLeft)
    
end