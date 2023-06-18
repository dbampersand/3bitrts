function setup()
    AbilitySetCastType(ABILITY_INSTANT);
    AbilitySetPortrait("assets/enemies/cusith/ability_howl.png");
    SetDescription("[b]Howl");
    SetCooldown(5)

end

function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/enemies/cusith/audio/howl.wav",1)

    local f1 = {};
    f1["trigger"] = TRIGGER_TIMER;
    f1["duration"] = 0.2
    f1["triggersPerSecond"] = 1
    f1["type"] = EFFECT_HURT_PERCENT;
    f1["value"] = 0.34;  
    f1["name"] = "Howl"
    f1["overwrites"] = true

    CreateCircularProjectiles(GetX(GetObjRef()),GetY(GetObjRef()),"",ATTACK_PROJECTILE_ANGLE,80,999,false,ATTACK_HITS_ENEMIES,128,COLOR_DAMAGE,0,{f1})

    return true; 
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft)

end
function onchanneled(obj)
    SetChannelingSprite("assets/enemies/cusith/cusith_channelling.png")
end