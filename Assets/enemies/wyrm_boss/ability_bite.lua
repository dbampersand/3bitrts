function setup()
    SetAbilityRange(2)
    SetAbilityCooldown(20);
    AbilitySetPortrait("Assets/enemies/wyrm_boss/ability_bite.png");
    SetDescription("[b]Bite\n\nDeals damage over time to the currently attacked target.")

end

function casted(x,y,obj,headingx,headingy)
    if (obj == GetThisObj()) then
        return false;
    end

    f1 = {};
    f1["trigger"] = TRIGGER_TIMER;
    f1["type"] = EFFECT_HURT;
    f1["triggersPerSecond"] = 5
    f1["value"] = 2;  
    f1["duration"] = 10;

    ApplyEffect(obj,{f1});
    return true; 
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft)

end