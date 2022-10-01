function setup()
    SetAbilityRange(999)
    SetCooldown(15)
    AbilitySetPortrait("assets/enemies/minotaur/ability_charge.png");
    SetDescription("[b]Charge\n\nRushes the target, dealing damage around")
    SetAbilityHint(HINT_LINE);
    SetAbilityTargetHint(Bor(ABILITY_POINT,ABILITY_TARGET_FRIENDLY));
end

function casted(x,y,obj,headingx,headingy)

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 10;  

    local speed = {}
    speed["trigger"] = TRIGGER_CONST
    speed["type"] = EFFECT_SPEED
    speed["duration"] = 1
    speed["value"] = 200

    SetObjChannelling(GetObjRef(),10);

    atk = CreateAOE(GetX(GetObjRef()),GetY(GetObjRef()),"", 50, 0.1, 1, true, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_NONE, false, GetObjRef(), {f1})
    ApplyEffect(GetObjRef(),{speed});
    
    local x = (GetX(obj) - GetX(GetObjRef())) * 256;
    local y = (GetY(obj) - GetY(GetObjRef())) * 256;
    SetMovePoint(GetObjRef(),x,y,false,false);
    SetObjChannelling(GetObjRef(),1);

    return true; 
end

function onhit(x,y,objhit)

end
function ontimeout(x,y,obj,dt,target)
    atk = -1
end


function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
    if (atk >= 0 and attackRef == atk) then
        x2 = GetX(parent);
        y2 = GetY(parent);
        MoveAttack(atk,x2,y2);
    end

end