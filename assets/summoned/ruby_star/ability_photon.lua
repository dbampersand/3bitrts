local damage = 200
local speed = 90

function setup()
    SetAbilityRange(64) 
    SetCooldown(8);
    AbilitySetCastType(ABILITY_POINT);

    AbilitySetPortrait("assets/enemies/kobold_miner/ability_throw_bomb.png");
    SetDescription("[b]Photon\n\nFires a damaging projectile.");
    SetAbilityName("Photon"); 

end

function casted(x,y,obj,headingx,headingy)
    
    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT
    f1["type"] = EFFECT_HURT
    f1["value"] = damage;


    projectile = CreateProjectile(GetX(obj),GetY(obj),x,y,"",ATTACK_PROJECTILE_ANGLE,speed,999,false,ATTACK_HITS_ENEMIES,COLOR_FRIENDLY_DAMAGE,1,{f1})


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