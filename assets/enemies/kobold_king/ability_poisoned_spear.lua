function setup()
    SetAbilityRange(999)
    SetCooldown(12)
    AbilitySetPortrait("assets/enemies/minotaur/ability_chuck.png");
    SetDescription("[b]Poisoned Spear\n\nStabs the target with a poison-tipped spear, dealing damage and poisoning them.")
    SetAbilityHint(HINT_LINE);
end

function casted(x,y,obj,headingx,headingy)

    f1 = {};
    f1["trigger"] = TRIGGER_TIMER;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 20;  
    f1["triggersPerSecond"] = 1
    f1["duration"] = 10

    f2 = {};
    f2["trigger"] = TRIGGER_INSTANT;
    f2["type"] = EFFECT_HURT;
    f2["value"] = 999;  

    ApplyEffect(obj,{f1,f2});

    return true; 
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft)

end