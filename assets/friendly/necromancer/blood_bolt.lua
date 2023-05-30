local duration = 12;
local aoe = -1

local damage = 10000


function setup()
    AbilitySetPortrait("assets/friendly/bard/icon_aoe_shield.png");
    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityRange(80)

    SetCooldown(99999999)
    SetCooldownTimer(99999999);

    SetAbilityMaxStacks(3)
    SetAbilityStacks(0)
    SetDescription("Blood Bolt\n\nDeals damage to a unit. When this ability kills, it generates a stack of ...")
    
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/bard/audio/song_of_shield.wav",1)

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = damage;
    ApplyEffect(obj,{f1});

    if (IsAlive(obj)) then
    end
    return true;
end
function untoggle()
end


function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
end

function effecttick(effectType,value,obj,x,y)
    if (IsAlive(obj) == false) then
        CreateObject("assets/friendly/necromancer/add/skeleton_melee.lua",x,y,TYPE_FRIENDLY,0.5)

    end
end 