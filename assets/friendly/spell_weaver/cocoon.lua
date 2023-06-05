local duration = 12;
local aoe = -1

local damage = 10000


function setup()
    AbilitySetPortrait("assets/friendly/bard/icon_aoe_shield.png");
    AbilitySetCastType(ABILITY_TARGET_FRIENDLY);
    SetAbilityRange(80)

    SetCooldown(99999999)
    SetCooldownTimer(99999999);

    SetAbilityMaxStacks(3)
    SetAbilityStacks(0)
    SetDescription("Cocoon\n\nWraps the target in enchanted fabric, giving them armor and regenerating health, but also stunning them.")
    
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/bard/audio/song_of_shield.wav",1)

    local f1 = {};
    f1["trigger"] = TRIGGER_CONST;
    f1["type"] = EFFECT_ARMOR;
    f1["value"] = armor;
    f1["duration"] = duration

    local f2 = {};
    f2["trigger"] = TRIGGER_INSTANT;
    f2["type"] = EFFECT_STUN;
    f2["value"] = duration;


    local f3 = {};
    f3["trigger"] = TRIGGER_TIMER;
    f3["type"] = EFFECT_HEAL_PERCENT;
    f3["value"] = 120 / duration;
    f3["duration"] = duration


    ApplyEffect(obj,{f1,f2,f3});

    AddStack(GetObjRef(),3,-1)

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