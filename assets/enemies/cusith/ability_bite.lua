local cone = -1
local objTargetting = -1

local radius = 80
local length = 140

function setup()

    SetAbilityRange(500)
    --AbilitySetCastType(ABILITY_CONE);
    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityHint(HINT_CONE,radius,false,length);
    SetCooldown(4); 
    AbilitySetPortrait("assets/enemies/cusith/ability_bite.png");
    SetAbilityName("Bite")
    SetDescription("[b]Bite\n\nDeals damage in a cone.")

end
function casted(x,y,obj,headingx,headingy)
    if (obj == GetThisObj()) then
        return false;
    end
    PlaySound("assets/enemies/cusith/audio/bite.wav",1)


    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 100;  

    center = GetCentre(GetObjRef());
    centertarget = GetCentre(obj);
    objTargetting = obj;
    cone = CreateCone(center.x,center.y,centertarget.x,centertarget.y,"", radius, 0, 0, false, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_HORIZONTAL_QUARTER,length, {f1})
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

function onchanneled(obj, channelingtime, totalchanneltime, targetObj, targetX, targetY, targetHeadingX, targetHeadingY)
    SetChannelingSprite("assets/enemies/cusith/cusith_channelling_bite.png")

end