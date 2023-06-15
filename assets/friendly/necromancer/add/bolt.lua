local duration = 12;
local aoe = -1

local damage = 80


function setup()
    AbilitySetPortrait("assets/friendly/bard/icon_aoe_shield.png");
    AbilitySetCastType(ABILITY_POINT);
    SetAbilityRange(80)
    SetCooldown(10)
    SetDescription("Bolt\n\nFires a damaging projectile.")
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/necromancer/add/audio/bolt.wav",1)

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = damage;

    projectile = CreateProjectile(GetX(GetObjRef()),GetY(GetObjRef()),x,y,"",ATTACK_PROJECTILE_ANGLE,25,4,true,ATTACK_HITS_ENEMIES,COLOR_DAMAGE,1,{f1})

    return true;
end
function untoggle()
end


function onhit(x,y,objhit)
    PlaySound("assets/friendly/necromancer/add/audio/bolt_hit.wav",0.5)
end

function abilitytick(x, y, durationLeft)
end

function effecttick(effectType,value,obj)
end 

