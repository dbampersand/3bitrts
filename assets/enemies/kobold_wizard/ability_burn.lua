local damage = 125
local time = 12
local ticksPerSecond = 1;

function setup()
    SetAbilityRange(64)
    SetCooldown(24);
    AbilitySetPortrait("assets/enemies/kobold_wizard/ability_burn.png");
    SetDescription("[b]Burn\n\nDeals damage over time to the target.");
    SetAbilityName("Burn"); 

end

function casted(x,y,obj,headingx,headingy)
    

    local f1 = {};
    f1["trigger"] = TRIGGER_TIMER; 
    f1["type"] = EFFECT_HURT;
    f1["value"] = damage / time * ticksPerSecond;
    f1["duration"] = time;
    f1["triggersPerSecond"] = 1;

    ApplyEffect(obj,{f1});

    return true; 
end

function onhit(x,y,objhit)
end
function ontimeout(x,y,obj,dt,target)
end
function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
    
end
function onchanneled() 
end