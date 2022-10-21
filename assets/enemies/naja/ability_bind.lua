local duration = 2


function setup()

    SetAbilityRange(999)
    --AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityHint(HINT_LINE);
    SetCooldown(16); 
    AbilitySetPortrait("assets/enemies/viper/ability_bite.png");
    SetDescription("[b]Bind\n\nStuns the target.")
end
function casted(x,y,obj,headingx,headingy)


    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_STUN;
    f1["value"] = duration;

    CreateProjectile(GetX(GetObjRef()),GetY(GetObjRef()),GetX(obj),GetY(obj),"",ATTACK_PROJECTILE_POINT,25,10,false,ATTACK_HITS_ENEMIES,COLOR_POISON,{f1})


    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
end