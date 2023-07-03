local duration = 12
local dps = 30
local tickRate = 0.5

local radius = 25

function setup()
    SetAbilityRange(200)
    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityHint(HINT_CIRCLE,radius);
    SetCooldown(20); 
    AbilitySetPortrait("assets/enemies/skeleton_minion_ranged/ability_rain.png");
    SetDescription("[b]Rain\n\nShoots a volley of arrows, slowing and damaging enemies in an area.")
end
function casted(x,y,obj,headingx,headingy)

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = dps * tickRate;
    f1["stacks"] = false

    local f2 = {};
    f2["trigger"] = TRIGGER_CONST;
    f2["type"] = EFFECT_SPEED;
    f2["value"] = -20;
    f2["duration"] = tickRate
    f2["stacks"] = false


    local atk = CreateAOE(GetX(obj),GetY(obj),"", radius, tickRate, duration, true, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_DAMAGE_EIGTH, false, -1,0,0,0, {f1,f2})
    
    local dir = GetHeadingVector(GetX(GetObjRef()), GetY(GetObjRef()),GetX(obj),GetY(obj))
    SetAttackMoveAngle(atk,dir.headingx,dir.headingy)
    SetAttackVelocity(atk,30)
    for i = 1, 4 do
        After(PlaySound,RandRange(0,0.15),true,"assets/enemies/skeleton_minion_ranged/audio/rain.wav",RandRange(0.1,0.35),GetX(obj),GetY(obj));
    end
    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
end
function applyattack(a,x,y) 
    PlaySound("assets/enemies/skeleton_minion_ranged/audio/rain.wav",0.1,x,y)
end