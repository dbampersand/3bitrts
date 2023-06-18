function setup()
    SetAbilityRange(999)
    SetCooldown(12)
    AbilitySetPortrait("assets/enemies/kobold_king/ability_poisoned_spear.png");
    SetDescription("[b]Poisoned Spear\n\nStabs the target with a poison-tipped spear, dealing damage and poisoning them.")
    SetAbilityHint(HINT_LINE);

    SetAbilityName("Poisoned Spear"); 

end

function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/enemies/kobold_king/audio/poisoned_spear.wav",1)

    local f1 = {};
    f1["trigger"] = TRIGGER_TIMER;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 20;  
    f1["triggersPerSecond"] = 1
    f1["duration"] = 10

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
function onchanneled() 
    SetChannelingSprite("assets/enemies/kobold_king/kobold_king_channelling_poisoned_spear.png");
end