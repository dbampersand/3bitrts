function setup()
    SetAbilityRange(2)
    SetCooldown(20);
    AbilitySetPortrait("assets/enemies/wyrm_boss/ability_bite.png");
    SetDescription("[b]Bite\n\nDeals damage over time to the currently attacked target.")
    SetAbilityHint(HINT_LINE);
end

function casted(x,y,obj,headingx,headingy)
    if (obj == GetThisObj()) then
        return false;
    end

    f1 = {};
    f1["trigger"] = TRIGGER_TIMER;
    f1["type"] = EFFECT_HURT;
    f1["triggersPerSecond"] = 5
    f1["value"] = 2.5;  
    f1["duration"] = 10;
    f1["portrait"] = "assets/enemies/wyrm_boss/ability_bite_effect_portrait.png"
    
    f2 = {};
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