
function setup()
    AbilitySetPortrait("assets/friendly/arcane_mage/mana_sword/icon_empower.png");
    AbilitySetCastType(ABILITY_INSTANT);
    SetDescription("Empower\n\nAdds attack damage.");
    SetCooldown(4);
    SetAbilityRange(50);    
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/arcane_mage/audio/empower.wav",1)

    f1 = {};
    f1["trigger"] = TRIGGER_CONST
    f1["type"] = EFFECT_ATTACKDAMAGE
    f1["value"] = 10;
    f1["duration"] = 6

    ApplyEffect(obj,{f1})

    return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end