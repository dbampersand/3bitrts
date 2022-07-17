function setup()
    SetAbilityRange(2)
    SetCooldown(10)
    SetAbilityHint(HINT_CIRCLE,30);
    AbilitySetCastType(ABILITY_POINT);
    AbilitySetPortrait("assets/enemies/wyrm_boss/ability_nuke.png");
    SetDescription("[b]Nuke\n\nDeals massive soak damage in a circular area after 10 seconds.")

end

function casted(x,y,obj,headingx,headingy)

    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    --f1["triggersPerSecond"] = 1 
    f1["value"] = 200;  
    --f1["duration"] = 4;

    CreateAOE(GetX(obj),GetY(obj),"", 30, 2, 2, false, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_HORIZONTAL_QUARTER, true, obj, {f1})
    return true; 
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft, obj, target)
    MoveAttack(GetX(target),GetY(target));
    
end