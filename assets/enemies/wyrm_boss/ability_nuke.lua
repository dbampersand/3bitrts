function setup()
    SetAbilityRange(256)
    SetCooldown(10)
    SetAbilityHint(HINT_CIRCLE,30);
    AbilitySetCastType(ABILITY_POINT);
    AbilitySetPortrait("assets/enemies/wyrm_boss/ability_nuke.png");
    SetAbilityName("Nuke");
    SetDescription("[b]Nuke\n\nDeals massive soak damage in a circular area after 10 seconds.")

end

function casted(x,y,obj,headingx,headingy)


    PlaySound("assets/enemies/wyrm_boss/audio/nuke.wav",1)

    f1 = {};
    f1["trigger"] = TRIGGER_TIMER;
    f1["type"] = EFFECT_HURT;
    f1["triggersPerSecond"] = 1 
    f1["value"] = 20;  
    f1["duration"] = 4;

    CreateAOE(GetX(obj),GetY(obj),"", 30, 6, 6, false, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_HORIZONTAL_QUARTER, true, obj, {f1})
    return true; 
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft, obj, target)
    MoveAttack(GetAttackRef(),GetX(target),GetY(target));
    
    
end
function applyattack(a,x,y)
    PlaySound("assets/enemies/wyrm_boss/audio/nuke_tick.wav",1,x,y)
end