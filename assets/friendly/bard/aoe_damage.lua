local speed = 50;
local duration = 2;
local aoe = -1

local radius = 30


function setup()
    AbilitySetPortrait("assets/friendly/bard/icon_aoe_damage.png");
    AbilitySetCastType(ABILITY_TOGGLE);
    AbilitySetHintColor(EFFECT_HURT)
    SetAbilityRange(duration * speed)
    SetCooldown(10)
    SetDescription("Claidrich\n\nDoes damage in a radius.")
    
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/bard/audio/aoe_damage.wav",1)

    UntoggleOthers();
    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 10;
    f1["overwrites"] = true
    f1["name"] = "Song of Damage"


    aoe = CreateAOE(GetX(GetObjRef()),GetY(GetObjRef()),"", radius, 1, 1, false, ATTACK_HITS_ENEMIES,COLOR_FRIENDLY_DAMAGE,DITHER_DAMAGE_QUARTER, false,-1,0,0,0,{f1})
    SetAttackCircle(aoe,true);
 
    return true;
end
function untoggle()
    RemoveAttack(aoe);
    aoe = -1;
    return true;
end

function onhit(x,y,objhit)
end


function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
    MoveAttack(aoe,GetX(parent),GetY(parent));
end
function applyattack(a,x,y)
end