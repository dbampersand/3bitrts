local duration = 15;
local tickrate = 1

local sps = 50

function setup()
    AbilitySetPortrait("assets/friendly/paladin/icon_circle_of_protection.png");
    AbilitySetCastType(ABILITY_INSTANT);
    SetAbilityRange(40)
    SetDescription("Circle of Protection\n\nGenerates a defensive circle, providing a shield around you.")
    SetCooldown(duration)
    SetManaCost(50)
end
function casted(x,y,obj,headingx,headingy)


    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_SHIELD;
    f1["value"] = sps * tickrate;  
    f1["duration"] = 2;  
    
    local aoe = CreateAOE(GetX(GetObjRef()),GetY(GetObjRef()),"",40,tickrate,duration,false,ATTACK_HITS_FRIENDLIES,COLOR_FRIENDLY_SHIELD,DITHER_VERTICAL_HALF,false,-1,0,0,0,{f1});


    PlaySound("assets/friendly/paladin/audio/circle_of_protection.wav",1);
    
    return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end
function applyattack(a,x,y)
    PlaySound("assets/friendly/paladin/audio/circle_of_protection_apply.wav",0.5,x,y);
end