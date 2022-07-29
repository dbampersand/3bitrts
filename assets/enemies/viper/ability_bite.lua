function setup()

    SetAbilityRange(2)
    --AbilitySetCastType(ABILITY_CONE);
    SetAbilityHint(HINT_LINE,80);
    SetCooldown(20); 
    AbilitySetPortrait("assets/enemies/viper/ability_bite.png");
    SetDescription("[b]Bite\n\nDeals damage over time.")

end
function casted(x,y,obj,headingx,headingy)

    local f1 = {};
    f1["trigger"] = TRIGGER_TIMER;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 2;
    f1["triggersPerSecond"] = 5;  
    f1["duration"] = 40;

    ApplyEffect(obj,{f1});
    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
end