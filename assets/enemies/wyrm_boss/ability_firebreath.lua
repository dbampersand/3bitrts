function setup()

    SetAbilityRange(256)
    --AbilitySetCastType(ABILITY_CONE);
    SetAbilityHint(HINT_CONE,80);
    SetCooldown(20); 
    AbilitySetPortrait("assets/enemies/wyrm_boss/ability_firebreath.png");
    SetAbilityName("Firebreath");
    SetDescription("[b]Firebreath\n\nA tremendous breath of fire, damaging units in a cone.")

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
    f1["value"] = 10;  
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
    SetAttackPosition(cone,GetX(GetObjRef()),GetY(GetObjRef()))
    SetAttackTarget(cone,objTargetting);
end

function applyattack(a,x,y)
    PlaySound("assets/enemies/wyrm_boss/audio/firebreath.wav",1)
end