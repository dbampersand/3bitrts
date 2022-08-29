local speed = 50;
local duration = 2;
local aoe = -1


function setup()
    AbilitySetPortrait("assets/friendly/bard/icon_aoe_damage.png");
    AbilitySetCastType(ABILITY_TOGGLE);
    SetAbilityRange(duration * speed)
    SetCooldown(10)
    SetDescription("Song of Damage\n\nDoes damage in a radius.")
    
end
function casted()
    UntoggleOthers();
    f1 = {};
    f1["trigger"] = TRIGGER_CONST;
    f1["type"] = EFFECT_DAMAGE;
    --f1["numTriggers"] = 5
    f1["value"] = 10;
    f1["duration"] = 1.5;
    f1["triggersPerSecond"] = 1
    f1["overwrites"] = true
    f1["name"] = "Song of Damage"


    aoe = CreateAOE(GetX(GetObjRef()),GetY(GetObjRef()),"", 30, 1, 10, false, ATTACK_HITS_FRIENDLIES,COLOR_DAMAGE,DITHER_HALF, false,-1,{f1})
    SetAttackCircle(aoe,true);
 
    return true;
end
function untoggle()
    --Print(aoe);
    RemoveAttack(aoe);
    aoe = -1;
    return true;
end

function onhit(x,y,objhit)
end


function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
    MoveAttack(aoe,GetX(parent),GetY(parent));
end