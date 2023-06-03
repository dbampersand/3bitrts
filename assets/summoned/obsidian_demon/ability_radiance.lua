local aoe = -1
local dps = 50
local ticksPerSec = 0.5

function setup()
    SetAbilityRange(256) 
    SetCooldown(20);
    AbilitySetCastType(ABILITY_INSTANT);
    AbilitySetPortrait("assets/enemies/kobold_miner/ability_throw_bomb.png");
    SetDescription("[b]Radiance\n\nUses the demon's own life to do area damage and threaten all enemies.");
    SetAbilityName("Radiance"); 

end

function casted(x,y,obj,headingx,headingy)
    
    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = dps * ticksPerSec;

    aoe = CreateAOE(x,y,"", 50, ticksPerSec, 20, false, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_DAMAGE_EIGTH, false, -1, {f1})
    return true; 
end

function onhit(x,y,objhit)
end
function ontimeout(x,y,obj,dt,target)
    aoe = -1;
end
function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
    SetAttackPosition(aoe,GetX(GetObjRef()),GetY(GetObjRef()))
    HurtObj(GetObjRef(),dps * dt)
end
function onchanneled() 
end