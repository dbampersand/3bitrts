function setup()
    AbilitySetPortrait("assets/friendly/warrior/icon_shield.png");
    AbilitySetCastType(ABILITY_INSTANT);
    SetDescription("Shield\n\nGives 50 shield")
    SetCooldown(15);
end

function casted()
    PlaySound("assets/friendly/warrior/audio/shield.wav",0.5);

    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_SHIELD;
    f1["value"] = 600;  
    f1["duration"] = 10;  
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