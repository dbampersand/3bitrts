local tickrate = 0.5
local duration = 12
local damage = 10

local cd = 0.25

local looseness = 6

function setup()

    SetAbilityRange(256)
    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityHint(HINT_NONE,80);
    SetCooldown(cd); 
    AbilitySetPortrait("assets/enemies/viper/ability_bite.png");
    SetDescription("[b]Peel\n\nReleases projectiles.")
end
function casted(x,y,obj,headingx,headingy)

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = damage;

    CreateProjectile(GetX(GetObjRef()),GetY(GetObjRef()),GetX(obj)+RandRange(-looseness,looseness),GetY(obj)+RandRange(-looseness,looseness),"",ATTACK_PROJECTILE_POINT,25,10,true,ATTACK_HITS_ENEMIES,COLOR_DAMAGE,1,{f1})



    -- PlaySound("assets/enemies/wolf/audio/call_pack.wav",0.35);
    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft,parent,target,dt,attackRef)
end

function applyattack(atk,x,y)
end