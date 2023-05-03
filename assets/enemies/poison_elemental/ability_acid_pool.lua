local duration = 120

local dps = 20
local tickrate = 0.5

local angle = 70
local numAoEs = 9

local radius = 20
local duration = 12


function setup()
    SetAbilityRange(256)
    SetCooldown(6);
    AbilitySetPortrait("assets/enemies/wraith/ability_throw_darts.png");
    SetDescription("[b]Acid Pool\n\nCreates an pool of acid.")
    SetAbilityName("Acid Pool"); 
    SetAbilityHint(HINT_CIRCLE,20);
    AbilitySetCastType(ABILITY_INSTANT);
end

function casted(x,y,obj,headingx,headingy)

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = dps * tickrate;  

    targets = GetRandomUnit(TYPE_ENEMY,TYPE_ALL,999,2)

    
    Print("gg");
    local numPools = Clamp(#targets,0,2)
    for i = 1, numPools do
        CreateAOE(GetX(targets[i]),GetY(targets[i]),"", radius, tickrate, duration, false, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_DAMAGE_QUARTER, false, obj, {f1})
    end
    

    PlaySound("assets/enemies/rock_tosser/audio/chuck.wav",1);



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