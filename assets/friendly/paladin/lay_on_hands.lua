local invincibleDuration = 3

function setup()
    AbilitySetPortrait("assets/friendly/paladin/icon_lay_on_hands.png");
    AbilitySetCastType(ABILITY_TARGET_FRIENDLY);
    AbilitySetHintColor(EFFECT_HEAL)


    SetDescription("Lay on Hands\n\nFully heals the target and makes them invincible for " .. invincibleDuration .. " seconds.")
    SetCooldown(50)
    SetManaCost(80);
    SetAbilityRange(15);


end

function casted(x,y,obj,headingx,headingy)
    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HEAL;
    f1["value"] = 100000;  

    local f2 = {};
    f2["trigger"] = TRIGGER_INSTANT;
    f2["type"] = EFFECT_INVULN
    f2["value"] = invincibleDuration;  



    ApplyEffect(obj,{f1});
    PlaySound("assets/friendly/paladin/audio/lay_on_hands.wav",1);
    ShakeScreen(2,0.1)
    return true;
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end