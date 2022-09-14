local delay = 5
local radius = 60

local length = 10

function setup()
    SetAbilityRange(256)
    SetCooldown(30);
    AbilitySetPortrait("assets/enemies/minotaur/ability_rampage.png");
    SetDescription("[b]Rampage\n\nMoves wildly, causing damage around it.")

end

local atk = -1
local attackActive = false;

function casted(x,y,obj,headingx,headingy)
    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 10;  

    atk = CreateAOE(GetX(GetObjRef()),GetY(GetObjRef()),"", radius, 0.1, length, true, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_NONE, false, GetObjRef(), {f1})
    
    return true; 
end

function onhit(x,y,objhit)

end
function ontimeout(x,y,obj,dt,target)
    EnableAI(GetObjRef(),true);

end
function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
    if (atk >= 0 and attackRef == atk) then
        x2 = GetX(target);
        y2 = GetY(target);
        MoveAttack(atk,x2,y2);
    end

    EnableAI(GetObjRef(),false);
    SetMovePoint(RandRange(0,255),RandRange(0,255),false,false);
end