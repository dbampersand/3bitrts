local radius = 25
local dps = 10
local duration = 10
local tickrate = 10

function setup()

    SetAbilityRange(999)
    --AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityHint(HINT_CIRCLE,radius);
    SetCooldown(16); 
    AbilitySetPortrait("assets/enemies/scorpion/ability_spit.png");
    SetDescription("[b]Spit\n\nSpits down an area of damaging acid.")
end
function casted(x,y,obj,headingx,headingy)


    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = dps / tickrate;

    local atk = CreateAOE(GetX(obj),GetY(obj),"", radius, 1/tickrate, duration, false, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_HORIZONTAL_QUARTER, false, obj, {f1})
    SetAttackInactive(atk,0.5)
    PlaySound("assets/enemies/scorpion/audio/spit.wav",1);
    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
end