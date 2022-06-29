function setup()
    AbilitySetCastType(ABILITY_TOGGLE);

    SetAbilityRange(20)
    SetCooldown(10);
    AbilitySetPortrait("assets/friendly/bard/icon_aoe_heal.png");
    SetDescription("Song of Healing\n\nProvides healing in a radius.")

end

local aoe = -1;
local g = 5;
function casted(x,y,obj,headingx,headingy)

    
    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HEAL;
    --f1["numTriggers"] = 5
    f1["value"] = 2;  
    --f1["duration"] = 10;
    f1["triggersPerSecond"] = 1

    aoe = CreateAOE(GetX(obj),GetY(obj),"", 30, 1, 10, false, ATTACK_HITS_FRIENDLIES,COLOR_HEAL,DITHER_HALF, {f1})

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

function abilitytick(x, y, durationLeft, obj, target)
    Print(aoe);
    --Print(g);


    MoveAttack(GetX(obj),GetY(obj));

end