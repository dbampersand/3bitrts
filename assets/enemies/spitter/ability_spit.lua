local damage = 5
local ticksPerSec = 5
local duration = 40

local radiusLooseAim = 30

local speed = 35
local duration = 8
function setup()

    SetAbilityRange(200)
    --AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityHint(HINT_LINE,80);
    SetCooldown(0.5); 
    SetAbilityMaxStacks(30,30)
    AbilitySetPortrait("assets/enemies/viper/ability_bite.png");
    SetDescription("[b]Bite\n\nDeals damage over time.")
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


    CreateProjectile(GetX(GetObjRef()),GetY(GetObjRef()),point.x,point.y,"",ATTACK_PROJECTILE_ANGLE,speed,duration,true,ATTACK_HITS_ENEMIES,COLOR_DAMAGE,{f1})

    PlaySound("assets/enemies/viper/audio/bite.wav",1);
    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
end