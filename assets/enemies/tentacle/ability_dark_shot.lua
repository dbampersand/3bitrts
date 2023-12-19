local damage = 20
local armourReduce = -3
local duration = 8

local speed = 60


function setup()

    SetAbilityRange(256)
    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityHint(HINT_LINE,80);
    SetCooldown(6); 
    AbilitySetPortrait("assets/enemies/tentacle/ability_dark_shot.png");
    SetDescription("[b]Dark Shot\n\nFires a damaging shot that reduces the targets' armour.")
end
function casted(x,y,obj,headingx,headingy)
    local f1 = {}
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = damage;

    local f2 = {}
    f2["trigger"] = TRIGGER_CONST;
    f2["type"] = EFFECT_ARMOR;
    f2["value"] = armourReduce;
    f2["duration"] = duration

    CreateProjectile(GetX(GetObjRef()),GetY(GetObjRef()),GetX(obj),GetY(obj),"",ATTACK_PROJECTILE_ANGLE,speed,9999,true,ATTACK_HITS_ENEMIES,COLOR_DAMAGE,2,{f1,f2})
    PlaySound("assets/enemies/tentacle/audio/ability_dark_shot.wav",0.5)

    return true; 
end

function onhit(x,y,objhit)
    PlaySound("assets/enemies/tentacle/audio/ability_dark_shot_hit.wav",0.5)

end
function ontimeout(x,y,parent,dt,target,attack)
end
function abilitytick(x, y, durationLeft,parent,target,dt,attackRef)
end

function applyattack(atk,x,y)
end
function timerbeforetick(t,func)
end