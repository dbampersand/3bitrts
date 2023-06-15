local duration = 3

function setup()
    AbilitySetPortrait("assets/friendly/rune_smith/icon_rune_of_shield.png");
    AbilitySetCastType(ABILITY_INSTANT);
    SetCooldown(5);

    SetDescription("Rune of Shield\n\nMakes you invulnerable for " .. duration .. " seconds.")
    SetCooldown(16)
    SetAbilityRange(15);


end

function casted(x,y,obj,headingx,headingy)
    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_INVULN;
    f1["value"] = 6;  


    ApplyEffect(obj,{f1});
    PlaySound("assets/friendly/rune_smith/audio/rune_of_shield.wav",1);

    return true;
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end