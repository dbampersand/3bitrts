function setup()
    AbilitySetPortrait("assets/friendly/warrior/icon_shield.png");
    AbilitySetCastType(ABILITY_INSTANT);
    SetDescription("Shield\n\nGives 50 shield")
    SetCooldown(15);
end

function casted()

    --SetObjPosition(GetObjRef(),xPos,yPos);
    --SetObjTargetPosition(GetObjRef(),xPos,yPos);
    --CreateProjectile(GetMouseX(),GetMouseY(),"",ATTACK_PROJECTILE_POINT,25,10,true,{})
    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_SHIELD;
    f1["value"] = 600;  
    f1["duration"] = 10;  
    ApplyEffect(GetThisObj(),{f1});

    return true;
end

function onhit(x,y,objhit)
    objRef = GetObjRef();
    SetObjPosition(GetObjRef(),x-GetWidthOf(objRef)/2,y-GetHeightOf(objRef)/2);
    SetObjTargetPosition(GetObjRef(),x-GetWidthOf(objRef)/2,y-GetHeightOf(objRef)/2);
end

function abilitytick(x, y, durationLeft)
    
end