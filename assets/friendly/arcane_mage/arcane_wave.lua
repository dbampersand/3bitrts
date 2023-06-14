local radius = 40
local length = 110

local manaReturn = 5
local numProjectiles = 8

local damage = 30
function setup()
    AbilitySetPortrait("assets/friendly/ranger/icon_cripple.png");
    AbilitySetCastType(ABILITY_POINT);
    SetAbilityRange(length)
    SetAbilityHint(HINT_CONE,radius,false,length)
    SetDescription("Arcane Wave\n\nCasts a wave of arcane missiles, returning mana for each that hit.")
    SetCooldown(12);
    SetManaCost(20)
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/arcane_mage/audio/arcane_wave.wav",1)
    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT
    f1["type"] = EFFECT_HURT
    f1["value"] = damage 

    CreateConeProjectiles(GetX(GetObjRef()),GetY(GetObjRef()),x,y,"",ATTACK_PROJECTILE_ANGLE,80,15,true,ATTACK_HITS_ENEMIES,10,COLOR_DAMAGE,radius,{f1});
    return true;
end
function onhit(x,y,objhit)
    AddMana(GetObjRef(),manaReturn)
end

function abilitytick(x, y, durationLeft)
    
end