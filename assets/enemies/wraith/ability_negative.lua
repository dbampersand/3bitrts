local duration = 0.1

local dps = 20
local tickrate = 0.5

local angle = 70
local numAoEs = 9

local radius = 40
local duration = 12


function setup()
    SetAbilityRange(256)
    SetCooldown(12);
    AbilitySetPortrait("assets/enemies/wraith/ability_negative.png");
    SetDescription("[b]Negative\n\nCreates an area of negative energy around the caster.")
    SetAbilityName("Desolate"); 
    SetAbilityHint(HINT_CIRCLE,20);
    AbilitySetCastType(ABILITY_INSTANT);
end

function casted(x,y,obj,headingx,headingy)

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = dps * tickrate;  

     CreateAOE(GetX(GetObjRef()),GetY(GetObjRef()),"", radius,  tickrate,duration, false, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_DAMAGE_QUARTER, false, obj,0,0,0, {f1})


    PlaySound("assets/enemies/wraith/audio/negative.wav",0.75);



    return true; 
end

function onhit(x,y,objhit)

end
function ontimeout(x,y,obj,dt,target)

end
function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
    
end
function onchanneled() 
    SetChannelingSprite("assets/enemies/wraith/wraith_casting_negative.png");

end