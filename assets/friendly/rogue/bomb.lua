local missChance = 40
local dps = 40
local tickrate = 0.5
local time = 6
function setup()
    AbilitySetPortrait("assets/friendly/rogue/icon_shiv.png");
    AbilitySetCastType(ABILITY_POINT);
    SetAbilityRange(60)
    SetDescription("Flash Bomb\n\nDeals damage and blinds enemy targets in the area, causing them to miss "..missChance .. "% of their attacks.")
    SetCooldown(14)
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/rogue/audio/shiv.wav",1.25)

    local f1 = {};
    f1["trigger"] = TRIGGER_CONST
    f1["type"] = EFFECT_MISS_CHANCE
    f1["value"] = 40;  
    f1["duration"] = tickrate

    local f2 = {};
    f2["trigger"] = TRIGGER_INSTANT
    f2["type"] = EFFECT_HURT
    f2["value"] = dps * tickrate;  


    CreateAOE(x,y,"", 30, tickrate, time, false, ATTACK_HITS_ENEMIES, COLOR_FRIENDLY_DAMAGE, DITHER_DAMAGE_HALF, false, obj, {f1,f2})

    return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end