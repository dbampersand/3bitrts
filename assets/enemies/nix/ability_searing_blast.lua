local aoe = -1
local direction = {};
local speed = 60;
local defaultSpeed = 30;
function setup()
    SetAbilityRange(256) 
    SetCooldown(4.5);
    AbilitySetPortrait("assets/enemies/kobold_miner/ability_throw_bomb.png");
    SetDescription("[b]Searing Blast\n\nShoots a ball that moves, dealing damage to all in its path.");
    SetAbilityName("Searing Blast"); 

end

function casted(x,y,obj,headingx,headingy)
    
    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 50;

    aoe = CreateAOE(x,y,"", 25, 1, 5, false, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_DAMAGE_EIGTH, false, -1, {f1})
    direction.x = GetX(obj) - GetX(GetObjRef()) 
    direction.y = GetY(obj) - GetY(GetObjRef()) 
    direction = Normalize(direction.x,direction.y);

    speed = defaultSpeed

    SetAttackMoveAngle(aoe,direction.x,direction.y)
    SetAttackVelocity(aoe,speed);

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
    SetChannelingSprite("assets/enemies/nix/nix_casting_searing_blast.png")
end