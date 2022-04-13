function setup()
    AbilitySetPortrait("Assets/friendly/Warrior/icon_taunt.png");
    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityCooldown(5);
    SetAbilityRange(40);

    SetDescription("Taunt\n\nTaunts the target, causing it to attack you for 10 seconds.")

end

function casted(x,y,obj,headingx,headingy)
    f1 = {};
    f1["trigger"] = TRIGGER_CONST;
    f1["type"] = EFFECT_THREAT;
    f1["value"] = 10000;  
    f1["duration"] = 10;  
    SetAttacking(obj);
    ApplyEffect(obj,{f1});
    return true;
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end