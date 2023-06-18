local length = 15
local time = 12
function setup()
    SetAbilityRange(512) 
    SetCooldown(20);
    AbilitySetPortrait("assets/enemies/kobold_crossbow/ability_shoot.png");
    SetDescription("[b]Shoot\n\nFires a bolt from their crossbow.");
    SetAbilityName("Shoot"); 
    SetAbilityHint(HINT_LINE);

end

function casted(x,y,obj,headingx,headingy)
    
    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 120;

     CreateProjectile(GetX(GetObjRef()),GetY(GetObjRef()),GetX(obj),GetY(obj),"",ATTACK_PROJECTILE_ANGLE,25,10,true,ATTACK_HITS_ENEMIES,COLOR_DAMAGE,1,{f1})
     PlaySound("assets/enemies/kobold_crossbow/audio/shoot.wav",0.5)

    return true; 
end

function onhit(x,y,objhit)
    PlaySound("assets/enemies/kobold_crossbow/audio/shoot_hit.wav",0.5)

end
function ontimeout(x,y,obj,dt,target,atk)
end
function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
    
end
function onchanneled() 
end