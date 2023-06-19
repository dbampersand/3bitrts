local amountAttackSpeed = -0.3
local tickRate = 1
local duration = 12

function setup()

    SetAbilityRange(200)
    AbilitySetCastType(ABILITY_INSTANT);
    SetAbilityHint(HINT_LINE,80);
    SetCooldown(20); 
    AbilitySetPortrait("assets/enemies/skeleton_minion_melee/ability_toll.png");
    SetDescription("[b]Toll\n\nCreates an area of bonus attack speed.")
end
function casted(x,y,obj,headingx,headingy)

    local f1 = {};
    f1["trigger"] = TRIGGER_CONST;
    f1["type"] = EFFECT_ATTACKSPEED;
    f1["value"] = amountAttackSpeed;
    f1["duration"] = tickRate
    f1["stacks"] = false

    CreateAOE(GetX(GetObjRef()),GetY(GetObjRef()),"",40,tickRate,duration,false,ATTACK_HITS_ENEMIES,COLOR_SPEED,DITHER_SPEED_QUARTER,false,-1,{f1});

    PlaySound("assets/enemies/skeleton_minion_melee/audio/toll.wav",0.5);
    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
end