
function setup()
    AbilitySetPortrait("assets/friendly/rogue/icon_shiv.png");
    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityRange(50)
    SetDescription("Deceptive Strike\n\nDeals damage and reduces the cooldown of stab by 2 seconds.")
    SetCooldown(8)
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/rogue/audio/shiv.wav",1.25)

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT
    f1["type"] = EFFECT_HURT
    f1["value"] = 80;  

    ApplyEffect(obj,{f1})   

    ReduceCooldown(GetObjRef(),0,2);

    

    return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end