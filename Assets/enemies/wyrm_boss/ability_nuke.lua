function setup()
    SetAbilityRange(2)
    SetAbilityCooldown(10)
    SetAbilityHint(HINT_CIRCLE,30);
    AbilitySetCastType(ABILITY_POINT);
    AbilitySetPortrait("Assets/enemies/wyrm_boss/ability_nuke.png");
    SetDescription("Nuke\n\nDeals massive damage in a circular area after 10 seconds.")

end

function casted(x,y,obj,headingx,headingy)

    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    --f1["triggersPerSecond"] = 1 
    f1["value"] = 20;  
    --f1["duration"] = 4;

    CreateAOE(GetX(obj),GetY(obj),"", 30, 10, 10, false, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_HORIZONTAL_QUARTER, {f1})
    return true; 
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft)

end