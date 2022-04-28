function setup()

    SetAbilityRange(2)
    --AbilitySetCastType(ABILITY_CONE);
    SetAbilityHint(HINT_CONE,80);
    SetAbilityCooldown(10); 

end

function casted(x,y,obj,headingx,headingy)
    if (obj == GetThisObj()) then
        return false;
    end

    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 50;  
    center = GetCentre(GetObjRef());
    centertarget = GetCentre(obj);
    CreateCone(center.x,center.y,centertarget.x,centertarget.y,"", 80, 10, 10, false, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_HORIZONTAL_QUARTER,80, {f1})
    return true; 
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft)

end