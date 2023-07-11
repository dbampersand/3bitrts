local duration = 8;
local aoe = -1

local AS = -0.2

local radius = 100

function setup()
    AbilitySetPortrait("assets/friendly/bard/icon_ostinato.png");
    AbilitySetCastType(ABILITY_INSTANT);
    AbilitySetHintColor(EFFECT_ATTACKSPEED)
    SetAbilityRange(100)
    SetCooldown(30)
    SetDescription("Ostinato\n\nIncrease the attack speed of all nearby friendly units.")
    
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/bard/audio/ostinato.wav",1)
    
    local f1 = {};
    f1["trigger"] = TRIGGER_CONST;
    f1["type"] = EFFECT_ATTACKSPEED;
    f1["value"] = AS;
    f1["duration"] = duration;
    f1["name"] = "Ostinato"
    f1["overwrites"] = true;

    aoe = CreateAOE(GetX(obj),GetY(obj),"", radius, 0.1, 0.1, false, ATTACK_HITS_FRIENDLIES,COLOR_SPEED,DITHER_STAR_HALF, false, -1,0,0,0, {f1})
    SetAttackCircle(aoe,true);
   
    return true;
end
function untoggle()
end


function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
end