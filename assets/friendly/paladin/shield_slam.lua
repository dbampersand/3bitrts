
function setup()
    AbilitySetCastType(ABILITY_TARGET_ENEMY); 
    AbilitySetPortrait("assets/friendly/paladin/icon_shield_slam.png");
    

    SetAbilityRange(60)
    SetCooldown(10);
    SetDescription("Shield Slam\n\nBashes the target with your shield, stunning and dealing damage. Generates mana and threatens enemies nearby.")
end

local aoe = -1;

function casted(x,y,obj,headingx,headingy)

    PlaySound("assets/friendly/paladin/audio/shield_slam.wav",1);
    
    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 60;  
    
    local f2 = {};
    f2["trigger"] = TRIGGER_INSTANT;
    f2["type"] = EFFECT_THREAT;
    f2["value"] = 100;  

    local f3 = {}
    f3["trigger"] = TRIGGER_INSTANT;
    f3["type"] = EFFECT_ADD_MANA;
    f3["value"] = 10;  

    ApplyEffect(obj,{f1,f2});
    ApplyEffect(GetObjRef(),{f3});



    f4 = {};
    f4["trigger"] = TRIGGER_INSTANT;
    f4["type"] = EFFECT_THREAT;
    f4["value"] = 400; 

    CreateAOE(GetX(obj),GetY(obj),"",70,0.1,0.1,false,ATTACK_HITS_ENEMIES,COLOR_FRIENDLY_DAMAGE,DITHER_NONE,false,-1,{f4});

    ShakeScreen(4,0.1)

    return true; 
end

function untoggle()
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft, obj, target)

    --MoveAttack(GetAttackRef(),GetX(obj),GetY(obj));

end