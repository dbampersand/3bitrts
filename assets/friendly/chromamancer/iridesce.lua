function setup()
    AbilitySetPortrait("assets/friendly/chromamancer/icon_iridesce.png");
    AbilitySetCastType(ABILITY_TARGET_FRIENDLY);
    SetDescription("Iridesce\n\nShields target and heals over time.")
    SetCooldown(0.5);
    SetManaCost(20)
end

function casted(x,y,obj,headingx,headingy)

    --SetObjPosition(GetObjRef(),xPos,yPos);
    --SetObjTargetPosition(GetObjRef(),xPos,yPos);
    --CreateProjectile(GetMouseX(),GetMouseY(),"",ATTACK_PROJECTILE_POINT,25,10,true,{})
    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_SHIELD;
    f1["value"] = 200;  
    f1["duration"] = 10;

    f2 = {};    
    f2["trigger"] = TRIGGER_TIMER;
    f2["type"] = EFFECT_HEAL;
    f2["value"] = 20;  
    f2["duration"] = 10;
    f2["triggersPerSecond"] = 1;



    ApplyEffect(obj,{f1,f2});

    return true;
end

function onhit(x,y,objhit)
    objRef = GetObjRef();
    SetObjPosition(GetObjRef(),x-GetWidthOf(objRef)/2,y-GetHeightOf(objRef)/2);
    SetObjTargetPosition(GetObjRef(),x-GetWidthOf(objRef)/2,y-GetHeightOf(objRef)/2);
end

function abilitytick(x, y, durationLeft)
    
end