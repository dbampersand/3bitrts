local radius = 80

local length = 0.1

local totalDotDamage = 150
local dotTime = 10

local atk = -1;

function setup()
    SetAbilityRange(256)
    SetCooldown(60);
    AbilitySetPortrait("assets/enemies/cusith/ability_spin.png");
    SetDescription("[b]Spin\n\nSpins around, whipping targets with its tail, causing a bleed effect.")

end

function casted(x,y,obj,headingx,headingy)

    PlaySound("assets/enemies/cusith/audio/spin.wav",1)

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 80;  

    local f2 = {};
    f2["trigger"] = TRIGGER_INSTANT;
    f2["type"] = EFFECT_TIMER;
    f2["value"] = totalDotDamage / dotTime; 
    f2["duration"] = dotTime
    f2["name"] = "Tail Whip";

    CreateAOE(GetX(GetObjRef()),GetY(GetObjRef()),"", radius, 0.1, length, true, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_NONE, false, GetObjRef(), {f1,f2})

    return true; 
end

function onhit(x,y,objhit)

end
function ontimeout(x,y,obj,dt,target)
    atk = -1

end
function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
    
end
function onchanneled() 
    SetChannelingSprite("assets/enemies/cusith/cusith_channelling_spin.png");
end