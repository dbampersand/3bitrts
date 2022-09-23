function setup()
    AbilitySetPortrait("assets/friendly/paladin/icon_holy_word.png");
    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetDescription("Rune of Pain\n\nDamages you and your target. Applies a large amount of threat.")
    SetCooldown(8);
end

function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/warrior/audio/shield.wav",0.5);

    --SetObjPosition(GetObjRef(),xPos,yPos);
    --SetObjTargetPosition(GetObjRef(),xPos,yPos);
    --CreateProjectile(GetMouseX(),GetMouseY(),"",ATTACK_PROJECTILE_POINT,25,10,true,{})
    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 200;  

    f2 = {}
    f2["trigger"] = TRIGGER_INSTANT;
    f2["type"] = EFFECT_THREAT;
    f2["value"] = 200;  

    ApplyEffect(obj,{f1,f2});
    ApplyEffect(GetObjRef(),{f1});

    return true;
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end