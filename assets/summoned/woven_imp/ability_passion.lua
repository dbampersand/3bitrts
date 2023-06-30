local quicken = 0.2
local duration = 32


function setup()
    SetAbilityRange(256) 
    SetCooldown(32);

    AbilitySetCastType(ABILITY_TARGET_FRIENDLY);
    AbilitySetPortrait("assets/summoned/woven_imp/ability_passion.png");
    SetDescription("[b]Passion\n\nThe target's abilities cool down " .. quicken * 100 .. "% quicker for " .. duration .. " seconds");
    SetAbilityName("Passion"); 

end

function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/summoned/woven_imp/audio/passion.wav",0.5)
    
    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_COOLDOWN_RATE;
    f1["value"] = quicken;
    f1["duration"] = duration;

    ApplyEffect(obj,{f1})
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