function setup()
    AbilitySetPortrait("Assets/friendly/Bard/icon_shield.png");
    AbilitySetCastType(ABILITY_POINT);

end

function casted()
    Print("gg");
    f1 = {};
    f1["trigger"] = TRIGGER_TIMER;
    f1["type"] = EFFECT_HEAL;
    f1["numTriggers"] = 5
    f1["value"] = 2;  
    f1["duration"] = 10;

    f2 = {};
    f2["trigger"] = TRIGGER_INSTANT
    f2["type"] = EFFECT_DAMAGE
    f2["numTriggers"] = 1 
    f2["value"] = 10;  

    CreateProjectile(GetMouseX(),GetMouseY(),"",ATTACK_PROJECTILE_TARGETED,50,{f1,f2})
end