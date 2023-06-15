local duration = 8;
local aoe = -1

local radius = 30

local totalDamage = 120
local duration = 10
local ticksPerSec = 2

function setup()
    AbilitySetPortrait("assets/friendly/bard/icon_aoe_damage.png");
    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityRange(50)
    SetCooldown(12)
    SetDescription("Seance\n\nShow the target a vision of their own death, stunning and dealing damage over time. If the target dies under this effect, they release a violent scream, damaging nearby enemy units.")
    
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/necromancer/audio/seance.wav",1)

    local f1 = {};
    f1["trigger"] = TRIGGER_TIMER;
    f1["duration"] = duration
    f1["triggersPerSecond"] = ticksPerSec
    f1["type"] = EFFECT_HURT;
    f1["value"] = totalDamage / duration / ticksPerSec
    f1["name"] = "Seance"
    f1["overwrites"] = true

        
    local f2 = {};
    f2["trigger"] = TRIGGER_INSTANT;
    f2["type"] = EFFECT_STUN;
    f2["value"] = duration
    f2["name"] = "Seance Stun"
    f2["overwrites"] = true

    ApplyEffect(obj,{f1,f2})

    return true;
end
function untoggle()
    return true;
end

function onhit(x,y,objhit)
end


function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
    
end
function effecttick(effectType,value,obj,x,y,name)
    if (IsAlive(obj) == false) then

        local f1 = {};
        f1["trigger"] = TRIGGER_INSTANT;
        f1["type"] = EFFECT_HURT;
        f1["value"] = 200

        CreateAOE(GetX(obj),GetY(obj),"", 60, 0.1, 0.1, false, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_HORIZONTAL_QUARTER, false, obj, {f1})
    end
end