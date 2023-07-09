
function setup()
    AbilitySetPortrait("assets/friendly/rogue/icon_deceptive_strike.png");
    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityRange(50)
    SetDescription("Deceptive Strike\n\nDeals damage and reduces the cooldown of Backstab by 2 seconds.")
    SetCooldown(8)
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/rogue/audio/deceptive_strike.wav",1)


    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT
    f1["type"] = EFFECT_HURT
    f1["value"] = 60;  

    ApplyEffect(obj,{f1})   

    ReduceCooldown(GetObjRef(),2,3);

    

    return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end