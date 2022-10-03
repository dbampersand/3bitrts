local cone = -1
local objTargetting = -1

function setup()

    SetAbilityRange(2)
    --AbilitySetCastType(ABILITY_CONE);
    SetAbilityHint(HINT_CONE,120);
    SetCooldown(20); 
    AbilitySetPortrait("assets/enemies/wyrm_boss/ability_firebreath.png");
    SetDescription("[b]Bite\n\nDeals damage in a cone.")

end
function casted(x,y,obj,headingx,headingy)
    if (obj == GetThisObj()) then
        return false;
    end
    PlaySound("assets/enemies/wyrm_boss/audio/firebreath.wav",1)


    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 200;  

    center = GetCentre(GetObjRef());
    centertarget = GetCentre(obj);
    objTargetting = obj;
    cone = CreateCone(center.x,center.y,centertarget.x,centertarget.y,"", 120, 0, 0, false, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_HORIZONTAL_QUARTER,140, {f1})
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

function onchanneled(obj, channelingtime, totalchanneltime, targetObj, targetX, targetY, targetHeadingX, targetHeadingY)
    SetChannelingSprite("assets/enemies/cusith/cusith_channelling_bite.png")
end