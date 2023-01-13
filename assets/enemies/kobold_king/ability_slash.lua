local coneSize = 60



function setup()
    SetAbilityRange(coneSize)
    SetCooldown(15);
    AbilitySetPortrait("assets/enemies/cusith/ability_focus.png");
    SetDescription("[b]Slash\n\nAfter channelling, hits in a cone.")
    SetAbilityHint(HINT_CONE,coneSize);

    
end

function casted(x,y,obj,headingx,headingy)


    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 120;  

    center = GetCentre(GetObjRef());
    centertarget = GetCentre(obj);

    cone = CreateCone(center.x,center.y,centertarget.x,centertarget.y,"", 60, 0, 0, false, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_HORIZONTAL_QUARTER,coneSize, {f1})

    return true;
end

function onhit(x,y,objhit)

end
function ontimeout(x,y,obj,dt,target, attack)
end

function abilitytick(x, y, timeUntilAttackTick, parent, target, dt, attackRef, totalAttackDurationLeft)

end