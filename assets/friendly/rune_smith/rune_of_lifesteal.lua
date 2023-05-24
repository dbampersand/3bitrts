local lifestealAmt = 2.5
local duration = 10

function setup()
    AbilitySetCastType(ABILITY_INSTANT); 
    AbilitySetPortrait("assets/friendly/rune_smith/icon_rune_of_lifesteal.png");
    

    SetAbilityRange(60)
    SetCooldown(duration);
    SetDescription("Rune of Lifesteal\n\nGrants you lifestealing.")
end

function casted(x,y,obj,headingx,headingy)

    PlaySound("assets/friendly/warrior/audio/charge.wav",0.5);
    
    f1 = {};
    f1["trigger"] = TRIGGER_CONST;
    f1["type"] = EFFECT_LIFESTEAL;
    f1["value"] = lifestealAmt;  
    f1["duration"] = duration

    ApplyEffect(GetObjRef(),{f1});

    return true; 
end

function untoggle()
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft, obj, target)
end