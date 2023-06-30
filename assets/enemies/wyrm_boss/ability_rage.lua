local casts = 0

function setup()
    AbilitySetCastType(ABILITY_INSTANT);
    AbilitySetPortrait("assets/enemies/wyrm_boss/ability_summon_adds.png");
    SetDescription("[b]Rage");
    SetAbilityName("Rage");
    SetCooldown(1)

end

function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/enemies/wyrm_boss/audio/rage.wav",1)

    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 40;  

    local offset = 0;
    if (math.fmod(casts,2) == 0) then
        offset = 90;
    end
    CreateCircularProjectiles(GetX(GetObjRef()),GetY(GetObjRef()),"",ATTACK_PROJECTILE_ANGLE,40,999,false,ATTACK_HITS_ENEMIES,10,COLOR_DAMAGE,offset,{f1})
    casts = casts + 1;
    return true; 
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft)

end