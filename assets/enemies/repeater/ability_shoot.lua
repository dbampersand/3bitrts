
function setup()
    SetAbilityRange(120)
    SetCooldown(1.5);
    AbilitySetPortrait("assets/enemies/repeater/ability_shoot.png");
    SetDescription("[b]Shoot\n\nFires a projectile.");
    SetAbilityName("Shoot"); 

end

function casted(x,y,obj,headingx,headingy)
    

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 100;

    projectile = CreateProjectile(GetX(GetObjRef()),GetY(GetObjRef()),GetX(GetObjRef())+headingx,GetY(GetObjRef())+headingy,"",ATTACK_PROJECTILE_ANGLE,15,10,true,ATTACK_HITS_ENEMIES,COLOR_DAMAGE,1,{f1})
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