local duration = 12;
local aoe = -1

local damage = 100


function setup()
    AbilitySetPortrait("assets/friendly/bard/icon_aoe_shield.png");
    AbilitySetCastType(ABILITY_TARGET_FRIENDLY);
    SetAbilityRange(80)
    SetCooldown(10) 
    SetDescription("Harvest Blood\n\nDeals damage to a friendly unit, and generates a stack of Blood Bolt.")
    
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/bard/audio/song_of_shield.wav",1)

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = damage;
    ApplyEffect(obj,{f1});
    AddStack(GetObjRef(),1,1);

    return true;
end
function untoggle()
end


function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
end