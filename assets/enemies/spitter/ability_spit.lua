local damage = 5
local ticksPerSec = 5
local duration = 40

local radiusLooseAim = 30

local speed = 35
local duration = 8

local aoeDPS = 2
local aoeTickrate = 0.5
function setup()

    SetAbilityRange(200)
    --AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityHint(HINT_LINE,80);
    SetCooldown(0.5); 
    SetAbilityMaxStacks(30,30)
    AbilitySetPortrait("assets/enemies/viper/ability_bite.png");
    SetDescription("[b]Spit\n\nSpits a large number of projectiles towards the target.")
end
function casted(x,y,obj,headingx,headingy)

    if (ObjIsValidIndex(obj) == false) then
        return false;
    end
    local x2 = GetX(obj);
    local y2 = GetY(obj);

    local angle = RandRange(-radiusLooseAim/2,radiusLooseAim/2)
    local point = RotatePoint(x2,y2,GetX(GetObjRef()),GetY(GetObjRef()),angle);
    
    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = damage;


    CreateProjectile(GetX(GetObjRef()),GetY(GetObjRef()),point.x,point.y,"",ATTACK_PROJECTILE_ANGLE,speed,duration,true,ATTACK_HITS_ENEMIES,COLOR_DAMAGE,1,{f1})

    PlaySound("assets/enemies/viper/audio/bite.wav",1);
    return true; 
end

function onhit(x,y,objhit)
    local f1 = {};
    f1["trigger"] = TRIGGER_TIMER;
    f1["type"] = EFFECT_HURT;
    f1["value"] = aoeDPS * aoeTickrate;
    f1["duration"] = 20
    f1["name"] = "Spit"
    f1["canStack"] = true
    f1["maxStacks"] = 20

    aoe = CreateAOE(x,y,"", 15, .75, 10,false,ATTACK_HITS_ENEMIES,COLOR_POISON,DITHER_DAMAGE_QUARTER,false, -1, {f1})
    PushObj(x,y,objhit,100,0.1)
end
function abilitytick(x, y, durationLeft)
end