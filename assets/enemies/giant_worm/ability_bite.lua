local dps = 20
local duration = 60
local ticksPerSec = 3

function setup()

    SetAbilityRange(200)
    --AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityHint(HINT_CIRCLE,radius);
    SetCooldown(14); 
    AbilitySetPortrait("assets/enemies/giant_worm/ability_bite.png");
    SetDescription("[b]Bite\n\nBites the target, injecting a toxin.")
end

function casted(x,y,obj,headingx,headingy)

    local f1 = {};
    f1["trigger"] = TRIGGER_TIMER;
    f1["type"] = EFFECT_HURT;
    f1["value"] = dps / ticksPerSec;
    f1["ticksPerSecond"] = ticksPerSec
    f1["duration"] = duration

    ApplyEffect(obj,{f1});




    PlaySound("assets/enemies/giant_worm/audio/bite.wav",0.75);
    return true; 
end

function onhit(x,y,objhit)
    SetChannelingSprite("assets/enemies/giant_worm/giant_worm_casting_bite.png");
end

function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
end

function parentdeath(x, y, durationLeft, parent, target, attackRef)
end