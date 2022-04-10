local speed = 50;
local duration = 2;


function setup()
    AbilitySetPortrait("Assets/friendly/Bard/icon_aoe_shield.png");
    AbilitySetCastType(ABILITY_TOGGLE);
    SetAbilityRange(duration * speed)
    SetAbilityCooldown(10)
    SetDescription("Song of Shielding\n\nProvides shielding in a radius.")
    
end
function casted()
    UntoggleOthers();
    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_SHIELD;
    --f1["numTriggers"] = 5
    f1["value"] = 10;
    f1["duration"] = 10;
    f1["triggersPerSecond"] = 1

    aoe = CreateAOE(GetX(obj),GetY(obj),"", 30, 1, 10, false, ATTACK_HITS_FRIENDLIES, {f1})
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