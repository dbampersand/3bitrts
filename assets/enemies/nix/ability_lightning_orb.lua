local aoe = -1
local direction = {};
local speed = 60;
local defaultSpeed = 30;

local triggersPerSec = 6
local dps = 25
function setup()
    SetAbilityRange(32) 
    SetCooldown(4);
    AbilitySetPortrait("assets/enemies/nix/ability_lightning_orb.png");
    SetDescription("[b]Lightning Orb\n\nShoots a ball that moves, dealing damage to all in its path.");
    SetAbilityName("Lightning Orb"); 

end

function casted(x,y,obj,headingx,headingy)


    
    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = dps / triggersPerSec;

    aoe = CreateAOE(x,y,"", 20, 1 / triggersPerSec, 5, false, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_DAMAGE_QUARTER, false, -1, {f1})
    direction.x = GetX(obj) - GetX(GetObjRef()) 
    direction.y = GetY(obj) - GetY(GetObjRef()) 
    direction = Normalize(direction.x,direction.y);
    speed = defaultSpeed;

    SetAttackMoveAngle(aoe,direction.x,direction.y)
    SetAttackVelocity(aoe,speed);
    PlaySound("assets/enemies/nix/audio/lightning_orb.wav",0.5)

    return true; 
end

function onhit(x,y,objhit)
end
function ontimeout(x,y,obj,dt,target)
    aoe = -1;
end
function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
    if (attackRef >= 0) then
        speed = speed + dt*2;
        SetAttackVelocity(aoe,speed);
    end
end
function onchanneled() 
    SetChannelingSprite("assets/enemies/nix/nix_casting_lightning_orb.png")

end
function applyattack(a,x,y)
    PlaySound("assets/enemies/nix/audio/lightning_orb_tick.wav",0.1,x,y)
end