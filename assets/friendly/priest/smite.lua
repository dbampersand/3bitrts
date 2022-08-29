
function setup()
    AbilitySetPortrait("assets/friendly/priest/icon_smite.png");
    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityRange(60)
    SetDescription("Smite\n\nHits the target for minor damage.")
    SetCooldown(10);
    SetManaCost(20)
    SetAbilityMaxStacks(2);
end
function casted(x,y,obj,headingx,headingy)
    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT
    f1["type"] = EFFECT_HURT
    f1["value"] = 20;  

    ApplyEffect(obj,{f1})
    return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end