
function setup()
    AbilitySetCastType(ABILITY_TARGET_ENEMY); 
    AbilitySetPortrait("assets/friendly/paladin/icon_shield_slam.png");
    

    SetAbilityRange(60)
    SetCooldown(10);
    SetDescription("Shield Slam\n\nBashes the target with your shield, stunning and dealing damage. Generates mana.")
end

local aoe = -1;

function casted(x,y,obj,headingx,headingy)

    PlaySound("assets/friendly/warrior/audio/charge.wav",0.5);
    
    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 60;  
    
    f2 = {};
    f2["trigger"] = TRIGGER_INSTANT;
    f2["type"] = EFFECT_THREAT;
    f2["value"] = 100;  

    f3 = {}
    f3["trigger"] = TRIGGER_INSTANT;
    f3["type"] = EFFECT_ADD_MANA;
    f3["value"] = 10;  

    ApplyEffect(obj,{f1,f2});
    ApplyEffect(GetObjRef(),{f3});

    return true; 
end

function untoggle()
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft, obj, target)

    --MoveAttack(GetAttackRef(),GetX(obj),GetY(obj));

end