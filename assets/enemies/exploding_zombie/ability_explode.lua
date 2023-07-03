local damage = 120
local radius = 50
local duration = 0.4
function setup()

    SetAbilityRange(200)
    --AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityHint(HINT_LINE,50);
    SetCooldown(6); 
    AbilitySetPortrait("assets/enemies/exploding_zombie/ability_explode.png");
    SetDescription("[b]Explode\n\nExplodes, killing themselves and also dealing damage around.")
end
function casted(x,y,obj,headingx,headingy)

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = damage;

    CreateAOE(GetX(GetObjRef()),GetY(GetObjRef()),"",radius,duration,duration,false,ATTACK_HITS_ENEMIES,COLOR_DAMAGE,DITHER_DAMAGE_EIGTH,false,-1,0,0,0,{f1});
    SetDecoration(GetObjRef(),true);
    SetLifetime(GetObjRef(),duration)

    After(ShakeScreen,duration,false,3,0.2)

    PlaySound("assets/enemies/exploding_zombie/audio/explode.wav",1);
    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
end