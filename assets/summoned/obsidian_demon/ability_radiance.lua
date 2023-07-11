local aoe = -1
local dps = 50
local ticksPerSec = 0.5

function setup()
    SetAbilityRange(256) 
    SetCooldown(20);
    AbilitySetCastType(ABILITY_INSTANT);
    AbilitySetHintColor(EFFECT_HURT)
    AbilitySetPortrait("assets/summoned/obsidian_demon/ability_radiance.png");
    SetDescription("[b]Radiance\n\nUses the demon's own life to do area damage and threaten all enemies.");
    SetAbilityName("Radiance"); 

end

function casted(x,y,obj,headingx,headingy)

    PlaySound("assets/summoned/obsidian_demon/audio/radiance.wav",1);

    
    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = dps * ticksPerSec;

    local f2 = {};
    f2["trigger"] = TRIGGER_INSTANT;
    f2["type"] = EFFECT_THREAT;
    f2["value"] = 9999;  

    aoe = CreateAOE(x,y,"", 50, ticksPerSec, 20, false, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_DAMAGE_EIGTH, false, -1,0,0,0, {f1,f2})
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
function parentdeath()
    RemoveAttack(aoe)
end
function onchanneled() 
end
function applyattack(a,x,y)
    PlaySound("assets/summoned/obsidian_demon/audio/radiance_tick.wav",0.25,x,y);
end