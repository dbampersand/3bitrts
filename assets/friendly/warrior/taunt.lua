function setup()
    AbilitySetPortrait("assets/friendly/warrior/icon_taunt.png");
    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetCooldown(5);
    SetAbilityRange(40);

    SetDescription("Taunt\n\nTaunts the target, causing it to attack you for 10 seconds.")

end

function casted(x,y,obj,headingx,headingy)
    f1 = {};
    f1["trigger"] = TRIGGER_CONST;
    f1["type"] = EFFECT_THREAT;
    f1["value"] = 10000;  
    f1["duration"] = 10;  
    SetAttacking(obj);
    
    ApplyEffect(obj,{f1});

    f2 = {};
    f2["trigger"] = TRIGGER_INSTANT;
    f2["type"] = EFFECT_THREAT;
    f2["value"] = 250;  


    CreateAOE(GetX(obj),GetY(obj),"", 60, 0.1, 0.1, false, ATTACK_HITS_ENEMIES, COLOR_FRIENDLY_DAMAGE, DITHER_NONE,false, -1, {f2});

    PlaySound("assets/friendly/warrior/audio/taunt.wav",0.5);

    return true;
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end