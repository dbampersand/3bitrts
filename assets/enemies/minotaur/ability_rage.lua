function setup()
    AbilitySetCastType(ABILITY_INSTANT);
    AbilitySetPortrait("assets/enemies/wyrm_boss/ability_summon_adds.png");
    SetAbilityName("Rage")
    SetDescription("[b]Rage");
    SetCooldown(1)

end

function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/enemies/wyrm_boss/audio/rage.wav",1)

    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 10;  

    CreateCircularProjectiles(GetX(GetObjRef()),GetY(GetObjRef()),"",ATTACK_PROJECTILE_ANGLE,40,999,false,ATTACK_HITS_ENEMIES,10,COLOR_DAMAGE,0,{f1})

    return true; 
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft)

end