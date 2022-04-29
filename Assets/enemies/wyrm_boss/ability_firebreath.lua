function setup()

    SetAbilityRange(2)
    --AbilitySetCastType(ABILITY_CONE);
    SetAbilityHint(HINT_CONE,80);
    SetAbilityCooldown(10); 

end
local cone = -1
local objTargetting = -1
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
    objTargetting = obj;
    cone = CreateCone(center.x,center.y,centertarget.x,centertarget.y,"", 80, 0, 0, false, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_HORIZONTAL_QUARTER,80, {f1})
    SetAttackTarget(cone,objTargetting);

    return true; 
end

function onhit(x,y,objhit)
    cone = -1;
end

function abilitytick(x, y, durationLeft)
    center = GetCentre(GetObjRef());
    SetAttackPosition(cone,center.x,center.y)
    SetAttackTarget(cone,objTargetting);
end