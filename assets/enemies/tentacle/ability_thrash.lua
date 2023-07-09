local damage = 5

local numToMake = 10
local timeBetween = 0.1
local projectilesToMake = 4

function setup()

    SetAbilityRange(256)
    AbilitySetCastType(ABILITY_INSTANT);
    SetAbilityHint(HINT_LINE,80);
    SetCooldown(12); 
    AbilitySetPortrait("assets/enemies/viper/ability_bite.png");
    SetDescription("[b]Thrash\n\nFlails about, spawning projectiles.")
end
function casted(x,y,obj,headingx,headingy)
    local f1 = {}
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = damage;

    for i = 1, numToMake do
        local angle = DegToRad(360 / numToMake * (i))   

        local xTo = math.cos(angle)*100
        local yTo = math.sin(angle)*100
        After(CreateConeProjectiles,i/numToMake,false,GetX(GetObjRef()),GetY(GetObjRef()),GetX(GetObjRef())+xTo,GetY(GetObjRef())+yTo,"",ATTACK_PROJECTILE_ANGLE,80,15,false,ATTACK_HITS_ENEMIES,projectilesToMake,COLOR_DAMAGE,360/numToMake,{f1});
    end
    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft,parent,target,dt,attackRef)
end

function applyattack(atk,x,y)
end
function timerbeforetick(t,func)

    if (func == CreateAOE) then
        UpdateTimerArgument(t,1,GetX(GetObjRef()))
        UpdateTimerArgument(t,2,GetY(GetObjRef()))
    end
end