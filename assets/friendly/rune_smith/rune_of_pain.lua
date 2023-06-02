function setup()
    AbilitySetPortrait("assets/friendly/rune_smith/icon_rune_of_pain.png");
    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetDescription("Rune of Pain\n\nDamages you and your target. Applies a large amount of threat.")
    SetCooldown(8);
end

function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/warrior/audio/shield.wav",0.5);

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 200;  

    local f2 = {}
    f2["trigger"] = TRIGGER_INSTANT;
    f2["type"] = EFFECT_THREAT;
    f2["value"] = 200;  

    ApplyEffect(GetObjRef(),{f1});
    ApplyEffect(obj,{f1,f2});

    return true;
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end