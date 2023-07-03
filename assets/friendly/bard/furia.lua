local duration = 12;
local aoe = -1

local damage = 30

local radius = 100

function setup()
    AbilitySetPortrait("assets/friendly/bard/icon_furia.png");
    AbilitySetCastType(ABILITY_INSTANT);
    SetAbilityRange(100)
    SetCooldown(50)
    SetDescription("Furia\n\nIncreases the attack damage of all nearby friendly units")
    
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/bard/audio/furia.wav",1)

    local f1 = {};
    f1["trigger"] = TRIGGER_CONST;
    f1["type"] = EFFECT_ATTACKDAMAGE;
    f1["value"] = damage;
    f1["duration"] = duration;
    f1["name"] = "Furia"
    f1["overwrites"] = true;

    aoe = CreateAOE(GetX(obj),GetY(obj),"", radius, 0.1, 0.1, false, ATTACK_HITS_FRIENDLIES,COLOR_FRIENDLY_DAMAGE,DITHER_STAR_HALF, false, -1,0,0,0, {f1})
    SetAttackCircle(aoe,true);
    After(ShakeScreen,0.1,true,2,0.1)    
   
    return true;
end
function untoggle()
end


function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
end