function setup()
    AbilitySetCastType(ABILITY_TOGGLE);

    SetAbilityRange(20)
    SetCooldown(0);

end

local aoe = -1;

function casted(x,y,obj,headingx,headingy)

    
    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HEAL;
    --f1["numTriggers"] = 5
    f1["value"] = 2;  
    --f1["duration"] = 10;
    f1["triggersPerSecond"] = 1

    aoe = CreateAOE(GetX(obj),GetY(obj),"", 30, 1, 10, false, ATTACK_HITS_FRIENDLIES,COLOR_HEAL,DITHER_HALF,false, -1, {f1})
    SetAttackCircle(aoe,true);

    return true; 
end

function untoggle()
    RemoveAttack(aoe);
    aoe = -1;
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft, obj, target)

    MoveAttack(GetAttackRef(),GetX(obj),GetY(obj));

end