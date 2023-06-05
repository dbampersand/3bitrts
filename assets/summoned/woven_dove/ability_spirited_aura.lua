local numEffects = 1
local duration = 32

local radius = 128
function setup()
    SetAbilityRange(256) 
    SetCooldown(32);

    AbilitySetCastType(ABILITY_INSTANT);
    AbilitySetPortrait("assets/enemies/kobold_miner/ability_throw_bomb.png");
    SetDescription("[b]Spirited Aura\n\nCures bad effects in an area. Kills the user.");
    SetAbilityName("Spirited Aura"); 

end

function casted(x,y,obj,headingx,headingy)
    
    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_CURE;
    f1["value"] = numEffects;

    CreateAOE(GetX(GetObjRef()),GetY(GetObjRef()),"", radius, 0.1, 0.1, false, ATTACK_HITS_FRIENDLIES,COLOR_HEAL,DITHER_HEAL_HALF,false, -1, {f1})
    KillObj(GetObjRef())
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