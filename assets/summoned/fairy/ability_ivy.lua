local triggersPerSec = 6
local dps = 25

local duration = 10

local slow = -30

function setup()
    SetAbilityRange(64) 
    SetCooldown(4);
    AbilitySetCastType(ABILITY_TARGET_ENEMY);

    AbilitySetPortrait("assets/summoned/fairy/ability_ivy.png");
    SetDescription("[b]Ivy\n\nSlows and damages an enemy.");
    SetAbilityName("Ivy"); 

end

function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/summoned/fairy/audio/ivy.wav",1);
    
    f1 = {};
    f1["trigger"] = TRIGGER_TIMER;
    f1["type"] = EFFECT_HURT;
    f1["value"] = dps;  
    f1["duration"] = duration
    f1["triggersPerSecond"] = 1
    f1["name"] = "Ivy"

    f2 = {};
    f2["trigger"] = TRIGGER_CONST;
    f2["type"] = EFFECT_SPEED;
    f2["value"] = slow;  
    f2["duration"] = duration
    f2["name"] = "Ivy"

    ApplyEffect(obj,{f1,f2})

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