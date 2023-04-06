local duration = 2


function setup()

    SetAbilityRange(999)
    --AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityHint(HINT_LINE);
    SetCooldown(16); 
    AbilitySetPortrait("assets/enemies/viper/ability_bite.png");
    SetDescription("[b]Bind\n\nShoves and stuns the target.")
end
function casted(x,y,obj,headingx,headingy)


    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_STUN;
    f1["value"] = duration;
    
    CreateProjectile(GetX(GetObjRef()),GetY(GetObjRef()),GetX(obj),GetY(obj),"",ATTACK_PROJECTILE_POINT,25,10,true,ATTACK_HITS_ENEMIES,COLOR_POISON,{f1})

    PlaySound("assets/enemies/naja/audio/bind_cast.wav",1);

    return true; 
end

function onhit(x,y,objhit)
    PlaySound("assets/enemies/naja/audio/bind.wav",1);
    Print(GetObjectName(GetObjRef()));
    PushObj(GetX(GetObjRef()),GetY(GetObjRef()),objhit,340,0.1);
    ShakeScreen(2,0.4);
end

function abilitytick(x, y, durationLeft)
end