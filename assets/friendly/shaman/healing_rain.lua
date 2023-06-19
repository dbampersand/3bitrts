local totalHeal = 900
local duration = 8

function setup()
    AbilitySetPortrait("assets/friendly/shaman/icon_healing_rain.png");
    AbilitySetCastType(ABILITY_INSTANT);
    SetAbilityRange(60)
    SetDescription("Healing Rain\n\nHeal over time in a very large area. Cannot move while casting.")
    SetCooldown(30);
    SetManaCost(60)
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/shaman/audio/healing_rain.wav",1);

    local f1 = {};
    f1["trigger"] = TRIGGER_CONST
    f1["type"] = EFFECT_SPEED
    f1["value"] = -9999;  
    f1["duration"] = duration;  


    ApplyEffect(GetObjRef(),{f1})

    local f2 = {};
    f2["trigger"] = TRIGGER_INSTANT
    f2["type"] = EFFECT_HEAL
    f2["value"] = totalHeal / duration;


    local aoe = CreateAOE(GetX(GetObjRef()),GetY(GetObjRef()),"",120,1,duration,false,ATTACK_HITS_FRIENDLIES,COLOR_FRIENDLY_HEAL,DITHER_HEAL_EIGTH,false,-1,{f2});
    return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end

function applyattack(a,x,y)
    PlaySound("assets/friendly/shaman/audio/healing_rain.wav",1,x,y);
end