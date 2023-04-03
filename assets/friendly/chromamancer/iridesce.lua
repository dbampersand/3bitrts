function setup()
    AbilitySetPortrait("assets/friendly/chromamancer/icon_iridesce.png");
    AbilitySetCastType(ABILITY_TARGET_ALL);
    SetDescription("Iridesce\n\nShields target and heals over time. When used on enemies, causes a damage over time effect.")
    SetCooldown(0.5);
    SetManaCost(20)

end

function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/chromamancer/audio/iridesce.wav",1)


    local thisObjSide = GetObjFriendliness(GetObjRef());
    local targetSide = GetObjFriendliness(obj);

    local f1 = {};
    local f2 = {};
    --if this is a friendly unit
    if (thisObjSide == targetSide) then
        f1["trigger"] = TRIGGER_INSTANT;
        f1["type"] = EFFECT_SHIELD;
        f1["value"] = 200;  
        f1["duration"] = 10;
    
        f2["trigger"] = TRIGGER_TIMER;
        f2["type"] = EFFECT_HEAL;   
        f2["value"] = 20;  
        f2["duration"] = 10;
        f2["triggersPerSecond"] = 1;
        f2["name"] = "Iridesce";
        f2["description"] = "Every second, adds 20 shield to the target."
        ApplyEffect(obj,{f1,f2});

    else 
        f1["trigger"] = TRIGGER_TIMER;
        f1["type"] = EFFECT_HURT;   
        f1["value"] = 15;  
        f1["duration"] = 10;
        f1["triggersPerSecond"] = 1;
        f1["name"] = "Iridesce";
        f1["description"] = "Every second, 20 damage applied to the target."
        ApplyEffect(obj,{f1});
    end


    return true;
end

function onhit(x,y,objhit)
    objRef = GetObjRef();
    SetObjPosition(GetObjRef(),x-GetWidth(objRef)/2,y-GetHeight(objRef)/2);
    SetObjTargetPosition(GetObjRef(),x-GetWidth(objRef)/2,y-GetHeight(objRef)/2);
end

function abilitytick(x, y, durationLeft)
    
end