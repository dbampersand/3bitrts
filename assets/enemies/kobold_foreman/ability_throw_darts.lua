local length = 15
local time = 12

function setup()
    SetAbilityRange(256)
    SetCooldown(8);
    AbilitySetPortrait("assets/enemies/kobold_foreman/ability_throw_darts.png");
    SetDescription("[b]Throw Darts\n\nThrows darts in a cone at the target.")
    SetAbilityName("Throw Darts"); 
    SetAbilityHint(HINT_CIRCLE,20);

end

function casted(x,y,obj,headingx,headingy)

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 15;  


    CreateConeProjectiles(GetX(GetObjRef()),GetY(GetObjRef()),GetX(obj),GetY(obj),"",ATTACK_PROJECTILE_ANGLE,80,15,false,ATTACK_HITS_ENEMIES,10,COLOR_DAMAGE,60,{f1});

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