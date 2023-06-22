    function setup()
    SetAbilityRange(999)
    SetCooldown(12)
    SetAbilityName("Chuck")
    AbilitySetPortrait("assets/enemies/minotaur/ability_chuck.png");
    SetDescription("[b]Chuck\n\nThrows a club at the target, stunning them.")
    SetAbilityHint(HINT_LINE);
end

function casted(x,y,obj,headingx,headingy)

    PlaySound("assets/enemies/minotaur/audio/chuck.wav",1)

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_STUN;
    f1["value"] = 2;  
    f1["portrait"] = "assets/enemies/wyrm_boss/ability_fire_effect_portrait"

    local f2 = {};
    f2["trigger"] = TRIGGER_INSTANT;
    f2["type"] = EFFECT_HURT;
    f2["value"] = 100;  

    ApplyEffect(obj,{f1,f2});

    return true; 
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft)

end