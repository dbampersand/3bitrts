local coneSize = 60
local cone = -1


function setup()
    SetAbilityRange(100)
    SetCooldown(15);
    AbilitySetPortrait("assets/enemies/kobold_king/ability_slash.png");
    SetDescription("[b]Slash\n\nSwings a sword, hitting all units in a cone.")
    SetAbilityHint(HINT_CONE,coneSize);

    SetAbilityName("Slash"); 

    
end

function casted(x,y,obj,headingx,headingy)

    PlaySound("assets/enemies/kobold_king/audio/slash.wav",1)

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT
    f1["value"] = 75;  

    center = GetCentre(GetObjRef());
    centertarget = GetCentre(obj);

    cone = CreateCone(center.x,center.y,centertarget.x,centertarget.y,"", 60, 0.5, 0.5 , false, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_HORIZONTAL_QUARTER,GetAbilityRange(GetObjRef(),GetAbilityRef()), {f1})

    return true;
end

function onhit(x,y,objhit)

end
function ontimeout(x,y,obj,dt,target, attack)
end

function abilitytick(x, y, timeUntilAttackTick, parent, target, dt, attackRef, totalAttackDurationLeft)

end
function onchanneled() 
    SetChannelingSprite("assets/enemies/kobold_king/kobold_king_channelling_slash.png");
end