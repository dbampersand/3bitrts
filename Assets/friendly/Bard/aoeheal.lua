function setup()
    AbilitySetCastType(ABILITY_TARGET_INSTANT);

    SetAbilityRange(20)
    SetAbilityCooldown(10);

end

function casted(x,y,obj,headingx,headingy)

    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HEAL;
    f1["numTriggers"] = 5
    f1["value"] = 2;  
    f1["duration"] = 10;

    CreateAOE(GetX(obj),GetY(obj),"", 30, 1, 10, false, ATTACK_HITS_FRIENDLIES, {f1})
    return true; 
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft, obj, target)
    MoveAttack(GetX(obj),GetY(obj));

end