local tickrate = 2.5
local duration = 12
local dps = 10
local projectileDamage = 10

local radius = 35

function setup()

    SetAbilityRange(10)
    AbilitySetCastType(ABILITY_INSTANT);
    SetAbilityHint(HINT_LINE,80);
    SetCooldown(9999); 
    AbilitySetPortrait("assets/enemies/selkie/ability_tide_pool.png");
    SetDescription("[b]Tide Pool\n\nCreates a tide pool, which causes damage around it and spits projectiles.")
end
function casted(x,y,obj,headingx,headingy)

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = dps * tickrate;

    local aoe = CreateAOE(GetX(GetObjRef()),GetY(GetObjRef()),"",radius,tickrate,duration,false,ATTACK_HITS_ENEMIES,COLOR_DAMAGE,DITHER_DAMAGE_HALF,false,-1,0,0,0,{f1});

    PlaySound("assets/enemies/selkie/audio/ability_tide_pool.wav",0.5);
    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
end

function applyattack(atk,x,y)
    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = projectileDamage;  
    CreateCircularProjectiles(x,y,"",ATTACK_PROJECTILE_ANGLE,10,999,false,ATTACK_HITS_ENEMIES,12,COLOR_DAMAGE,RandRange(0,360),{f1})
    PlaySound("assets/enemies/selkie/audio/ability_tide_pool_tick.wav",0.25);

end