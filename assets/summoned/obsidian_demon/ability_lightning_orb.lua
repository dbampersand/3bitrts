local aoe = -1
local direction = {};
local speed = 60;
local defaultSpeed = 30;

local triggersPerSec = 3
local dps = 25
function setup()
    SetAbilityRange(64) 
    SetCooldown(4);
    AbilitySetCastType(ABILITY_POINT);

    AbilitySetPortrait("assets/summoned/obsidian_demon/ability_lightning_orb.png");
    SetDescription("[b]Lightning Orb\n\nShoots a ball that moves, dealing damage to all in its path.");
    SetAbilityName("Lightning Orb"); 

end

function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/summoned/obsidian_demon/audio/lightning_orb.wav",1);
    
    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = dps / triggersPerSec;

    aoe = CreateAOE(x,y,"", 20, 1 / triggersPerSec, 5, false, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_DAMAGE_QUARTER, false, -1, {f1})
    direction.x = x - GetX(GetObjRef()) 
    direction.y = y - GetY(GetObjRef()) 
    direction = Normalize(direction.x,direction.y);
    speed = defaultSpeed;

    SetAttackMoveAngle(aoe,direction.x,direction.y)
    SetAttackVelocity(aoe,speed);
    return true; 
end

function onhit(x,y,objhit)
end
function ontimeout(x,y,obj,dt,target)
    aoe = -1;
    PlaySound("assets/summoned/obsidian_demon/audio/lightning_orb_timeout.wav",1);
    
end
function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
    if (attackRef >= 0) then
        speed = speed + dt*2;
        SetAttackVelocity(aoe,speed);
    end
end
function onchanneled() 
end
function applyattack(a,x,y)
    PlaySound("assets/summoned/obsidian_demon/audio/lightning_orb_tick.wav",0.25,x,y);
end