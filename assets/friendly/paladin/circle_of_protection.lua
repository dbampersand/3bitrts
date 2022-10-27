local speed = 50;
local duration = 15;



function setup()
    AbilitySetPortrait("assets/friendly/paladin/icon_circle_of_protection.png");
    AbilitySetCastType(ABILITY_INSTANT);
    SetAbilityRange(40)
    SetDescription("Circle of Protection\n\nGenerates a defensive circle, providing a shield around you.")
    SetCooldown(duration)
    SetManaCost(65)
end
function casted(x,y,obj,headingx,headingy)


    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_SHIELD;
    f1["value"] = 50;  
    f1["duration"] = 2;  
    
    local aoe = CreateAOE(GetX(GetObjRef()),GetY(GetObjRef()),"",40,0.25,duration,false,ATTACK_HITS_FRIENDLIES,COLOR_FRIENDLY_SHIELD,DITHER_VERTICAL_HALF,false,-1,{f1});


    PlaySound("assets/friendly/warrior/audio/strike.wav",0.5);
    
    return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end