local duration = 8;
local aoe = -1

local radius = 30

local totalDamage = 120
local duration = 10
local ticksPerSec = 2

local attackSpeed = -0.1

function setup()
    AbilitySetPortrait("assets/friendly/spell_weaver/icon_stitch.png");
    AbilitySetCastType(ABILITY_TARGET_ALL);
    AbilitySetHintColor(EFFECT_ATTACKSPEED)
    SetAbilityRange(50)
    SetCooldown(8)
    SetDescription("Stitch\n\nAdds a stack of Weave and increases target units' attack speed")
    
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/spell_weaver/audio/stitch.wav",1)

    local f1 = {};
    f1["trigger"] = TRIGGER_CONST;
    f1["duration"] = duration
    f1["type"] = EFFECT_ATTACKSPEED;
    f1["value"] = attackSpeed
    f1["name"] = "Stitch"
    f1["overwrites"] = true

    ApplyEffect(obj,{f1})

    AddStack(GetObjRef(),2,1)
    AddStack(GetObjRef(),3,1)
    ShakeScreen(1,0.1)
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

        CreateAOE(GetX(obj),GetY(obj),"", 60, 0.1, 0.1, false, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_HORIZONTAL_QUARTER, false, obj,0,0,0, {f1})
    end
end