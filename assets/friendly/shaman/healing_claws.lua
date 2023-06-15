local heal = 60
local numProjectiles = 12

local timeBetween = 0.02
local timeBetweenSwipes = 0.1

local radius = 30
local speed = 20


function setup()
    AbilitySetPortrait("assets/friendly/shaman/icon_healing_rain.png");
    AbilitySetCastType(ABILITY_POINT);
    SetAbilityRange(60)
    SetDescription("Healing Claws\n\nDashes to an area and claws the air, creating several slow-moving projectiles of healing energy.")
    SetCooldown(10);
    SetManaCost(60)
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/shaman/audio/healing_claws.wav",1)
    After(PlaySound,timeBetweenSwipes,"assets/friendly/shaman/audio/healing_claws.wav",1)

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HEAL;   
    f1["value"] = heal;

    local heading = GetHeadingVector(GetX(GetObjRef()),GetY(GetObjRef()),x,y)
    Teleport(GetObjRef(),x,y);


    local x2 = GetX(GetObjRef())+(heading.headingx*1000)
    local y2 = GetY(GetObjRef())+(heading.headingy*1000)
    After(CreateConeProjectiles,0,GetX(GetObjRef()),GetY(GetObjRef()),x2,y2,"",ATTACK_PROJECTILE_ANGLE,speed*0.95,15,true,ATTACK_HITS_FRIENDLIES,10,COLOR_HEAL,-radius,{f1});
    After(CreateConeProjectiles,timeBetweenSwipes,GetX(GetObjRef()),GetY(GetObjRef()),x2,y2,"",ATTACK_PROJECTILE_ANGLE,speed,15,true,ATTACK_HITS_FRIENDLIES,10,COLOR_HEAL,radius,{f2});

    return true;
end
function timerbeforetick(timer,func) 
    if (func == CreateConeProjectiles) then
        UpdateTimerArgument(timer,1,GetX(GetObjRef()))
        UpdateTimerArgument(timer,2,GetY(GetObjRef()))
    end
end
function onhit(x,y,objhit)
    PlaySound("assets/friendly/shaman/audio/healing_claws_hit.wav",0.25)

end

function abilitytick(x, y, durationLeft)
    
end