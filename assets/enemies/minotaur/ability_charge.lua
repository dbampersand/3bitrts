function setup()
    SetAbilityRange(999)
    SetCooldown(15)
    AbilitySetPortrait("assets/enemies/minotaur/ability_charge.png");
    SetDescription("[b]Charge\n\nRushes the target, dealing damage around")
    SetAbilityHint(HINT_CIRCLE);
end

function casted(x,y,obj,headingx,headingy)

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 2;  

    local speed = {}
    speed["trigger"] = TRIGGER_CONST
    speed["type"] = EFFECT_SPEED
    speed["duration"] = 1
    speed["value"] = 200

    SetObjChannelling(GetObjRef(),10);

    atk = CreateAOE(GetX(GetObjRef()),GetY(GetObjRef()),"", radius, 0.1, length, true, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_NONE, false, GetObjRef(), {f1})
    ApplyEffect(GetObjRef(),{speed});
    
    local x = (GetX(obj) - GetX(GetObjRef())) * 256;
    local y = (GetY(obj) - GetY(GetObjRef())) * 256;
    SetMovePoint(GetObjRef(),x,y,false,false);
    SetObjChannelling(GetObjRef(),1);

    return true; 
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft)

end