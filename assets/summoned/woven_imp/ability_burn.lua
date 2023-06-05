local damage = 50
local speed = 90

local aoe = -1

local duration = 0

local tickRate = 0.5
local radius = 35

function setup()
    SetAbilityRange(64) 
    SetCooldown(32);
    AbilitySetCastType(ABILITY_INSTANT);

    AbilitySetPortrait("assets/enemies/kobold_miner/ability_throw_bomb.png");
    SetDescription("[b]Immolate\n\nLight a fire, dealing damage to the owner and enemies nearby, exploding on death.");
    SetAbilityName("Immolate"); 

end

function casted(x,y,obj,headingx,headingy)

    if (DeathTimerIsSet(GetObjRef())) then
        duration = GetLifetime(GetObjRef()) - 0.1
    else
        duration = 12
    end
    
    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT
    f1["type"] = EFFECT_HURT
    f1["value"] = damage * tickRate;


    aoe = CreateAOE(GetX(GetObjRef()),GetY(GetObjRef()),"", radius, tickRate, duration, false, ATTACK_HITS_ENEMIES,COLOR_DAMAGE,DITHER_HALF,false, -1, {f1})
    

    return true; 
end

function onhit(x,y,objhit)
end
function ontimeout(x,y,obj,dt,target)
end
function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
    MoveAttack(attackRef,GetX(GetObjRef()),GetY(GetObjRef()));
    
    duration = duration - dt

    if (duration <= 0) then
        local f1 = {};
        f1["trigger"] = TRIGGER_INSTANT
        f1["type"] = EFFECT_HURT
        f1["value"] = damage*3;
    
        CreateAOE(GetX(GetObjRef()),GetY(GetObjRef()),"", 50, 0.1, 0.1, false, ATTACK_HITS_ENEMIES,COLOR_DAMAGE,DITHER_DAMAGE_HALF,true, -1, {f1})
        
    end
end
function onchanneled() 
end