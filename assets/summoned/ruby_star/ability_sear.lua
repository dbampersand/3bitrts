local dps = 50
local tickRate = 0.25
local radius = 40

local duration = 8
function setup()
    SetAbilityRange(256) 
    SetCooldown(20);
    SetAbilityHint(HINT_CIRCLE,radius);

    AbilitySetCastType(ABILITY_POINT);
    AbilitySetPortrait("assets/enemies/kobold_miner/ability_throw_bomb.png");
    SetDescription("[b]Sear\n\nPlaces a fiery area on the ground, damaging all units.");
    SetAbilityName("Sear"); 

end

function casted(x,y,obj,headingx,headingy)
    
    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = dps * tickRate;

    CreateAOE(x,y,"", radius, tickRate, duration, false, Bor(ATTACK_HITS_FRIENDLIES,ATTACK_HITS_ENEMIES),COLOR_DAMAGE,DITHER_DAMAGE_QUARTER, false, -1, {f1})
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