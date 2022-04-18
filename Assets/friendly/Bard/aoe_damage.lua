local speed = 50;
local duration = 2;
local aoe = -1


function setup()
    AbilitySetPortrait("Assets/friendly/Bard/icon_aoe_damage.png");
    AbilitySetCastType(ABILITY_TOGGLE);
    SetAbilityRange(duration * speed)
    SetAbilityCooldown(10)
    SetDescription("Song of Damage\n\nDoes damage in a radius.")
    
end
function casted()
    UntoggleOthers();
    f1 = {};
    f1["trigger"] = TRIGGER_CONST;
    f1["type"] = EFFECT_DAMAGE;
    --f1["numTriggers"] = 5
    f1["value"] = 500;
    f1["duration"] = 1      ;
    f1["triggersPerSecond"] = 1

    aoe = CreateAOE(GetX(obj),GetY(obj),"", 30, 1, 10, false, ATTACK_HITS_FRIENDLIES,COLOR_DAMAGE,DITHER_HALF, {f1})
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

function abilitytick(x, y, durationLeft)
    MoveAttack(GetX(obj),GetY(obj));
    
end