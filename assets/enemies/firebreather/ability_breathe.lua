local duration = 0.1
local atk = -1

local rad = 30
local range = 80
function setup()

    SetAbilityRange(range)
    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityHint(HINT_CONE,rad);
    SetCooldown(12); 
    AbilitySetPortrait("assets/enemies/firebreather/ability_breathe.png");
    SetDescription("[b]Breathe\n\nDeals damage in an cone.")
end
function casted(x,y,obj,headingx,headingy)

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 120;  

    PlaySound("assets/enemies/firebreather/audio/breathe.wav",0.5);
    
    cone = CreateCone(GetX(GetObjRef()),GetY(GetObjRef()),GetX(obj),GetY(obj),"", rad, 0, 0, false, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_HORIZONTAL_QUARTER,range, {f1})


    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
    if (atk >= 0 and attackRef == atk) then
        x2 = GetX(target);
        y2 = GetY(target);
        MoveAttack(atk,x2,y2);
    end

end

function ontimeout(x,y,obj,dt,target)
    atk = -1
    PlaySound("assets/enemies/rock_tosser/audio/toss_small.wav",1);
end
