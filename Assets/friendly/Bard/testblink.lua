function setup()
    AbilitySetPortrait("Assets/friendly/Bard/icon_shield.png");
    AbilitySetCastType(ABILITY_POINT);

end

function casted()

    objRef = GetObjRef();

    xPos = GetMouseX()-GetWidthOf(objRef)/2;
    yPos = GetMouseY()-GetHeightOf(objRef)/2;


    --SetObjPosition(GetObjRef(),xPos,yPos);
    --SetObjTargetPosition(GetObjRef(),xPos,yPos);
    Teleport(GetObjRef(),GetMouseX(),GetMouseY());
    --CreateProjectile(GetMouseX(),GetMouseY(),"",ATTACK_PROJECTILE_POINT,25,10,true,{})
    
end

function onhit(x,y,objhit)
    objRef = GetObjRef();
    SetObjPosition(GetObjRef(),x-GetWidthOf(objRef)/2,y-GetHeightOf(objRef)/2);
    SetObjTargetPosition(GetObjRef(),x-GetWidthOf(objRef)/2,y-GetHeightOf(objRef)/2);
end

function abilitytick(x, y, durationLeft)
    
end