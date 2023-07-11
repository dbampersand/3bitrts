local duration = 12;
local aoe = -1

local damage = 500


function setup()
    AbilitySetPortrait("assets/friendly/necromancer/icon_blood_bolt.png");
    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    AbilitySetHintColor(EFFECT_HURT)

    SetAbilityRange(80)

    SetCooldown(99999999)
    SetCooldownTimer(99999999);

    SetAbilityMaxStacks(3)
    SetAbilityStacks(0)
    SetDescription("Blood Bolt\n\nDeals damage to a unit. When this ability kills, it creates a skeleton.")
    
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/necromancer/audio/blood_bolt.wav",1)

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = damage;
    ApplyEffect(obj,{f1});

    ShakeScreen(1,0.1)
    
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
        ShakeScreen(4,0.25)
    end
end 