local tickRate = 0.5
local dps = 70

local duration = 8

local potency = 0.2

function setup()
    AbilitySetPortrait("assets/friendly/lancer/icon_stampede.png");
    AbilitySetCastType(ABILITY_INSTANT);
    SetDescription("Stampede\n\nAllows the user to push objects, while dealing damage around them. Increases ability potency by " .. potency*100 .. "%.")
    SetCooldown(25);

    SetAbilityRange(50)
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/lancer/audio/stampede.wav",0.5)
    
    f1 = {}
    f1["trigger"] = TRIGGER_INSTANT
    f1["type"] = EFFECT_HURT
    f1["value"] = dps * tickRate;

    local radius = GetWidth(g) / 2
    aoe = CreateAOE(GetX(obj),GetY(obj),"", radius, tickRate, duration, false, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_DAMAGE_HALF, false, obj, {f1})

    f2 = {}
    f2["trigger"] = TRIGGER_CONST
    f2["type"] = EFFECT_ABILITY_POTENCY
    f2["value"] = potency;

    ApplyEffect(obj,{f2});

    After(SetObjectPush,duration+0.1,false,false)
   return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft,parent,target,dt,attackRef)
    SetObjectPush(GetObjRef(),true)
    SetAttackPosition(attackRef,GetX(GetObjRef()),GetY(GetObjRef()))
    
end