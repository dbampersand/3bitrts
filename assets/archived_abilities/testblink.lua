function setup()
    AbilitySetPortrait("assets/friendly/bard/icon_shield.png");
    AbilitySetCastType(ABILITY_POINT);

end

function casted()

    objRef = GetObjRef();

    xPos = GetMouseX()-GetWidth(objRef)/2;
    yPos = GetMouseY()-GetHeight(objRef)/2;


    --SetObjPosition(GetObjRef(),xPos,yPos);
    --SetObjTargetPosition(GetObjRef(),xPos,yPos);
    Teleport(GetObjRef(),GetMouseX(),GetMouseY());
    --CreateProjectile(GetMouseX(),GetMouseY(),"",ATTACK_PROJECTILE_POINT,25,10,true,{})
    return true;
end

function onhit(x,y,objhit)
    objRef = GetObjRef();
    SetObjPosition(GetObjRef(),x-GetWidth(objRef)/2,y-GetHeight(objRef)/2);
    SetObjTargetPosition(GetObjRef(),x-GetWidth(objRef)/2,y-GetHeight(objRef)/2);
end

function abilitytick(x, y, durationLeft)
    
end