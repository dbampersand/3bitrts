local radius = 60
local aoe = -1;

local lifestealPercent = 0.4

local damage = 150
function setup()
    AbilitySetCastType(ABILITY_INSTANT);

    SetAbilityRange(20)
    SetCooldown(30)
    
    AbilitySetPortrait("assets/friendly/necromancer/icon_harvest.png");
    SetDescription("Harvest\n\nSwings a sickle around, damaging and returning a percentage as healing to you.")

end

function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/necromancer/audio/harvest.wav",1)

    local f1 = {};  
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = damage;  


    CreateAOE(GetX(GetObjRef()),GetY(GetObjRef()),"", radius, 0.2, 0.2, true, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_DAMAGE_HALF, false, 0, {f1})
    After(ShakeScreen,0.2,true,2,0.1)    
    
    return true; 
end

function untoggle()
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
end

function effecttick(effectType,value,obj,x,y)
    local v = value * lifestealPercent
    Heal(GetObjRef(),v);
end 