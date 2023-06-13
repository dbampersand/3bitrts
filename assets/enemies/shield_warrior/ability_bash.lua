
function setup()

    SetAbilityRange(40)
    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetCooldown(12); 
    AbilitySetPortrait("assets/enemies/shield_warrior/ability_bash.png");
    SetDescription("[b]Bash\n\nSmashes a target with their shield.")

end
function casted(x,y,obj,headingx,headingy)

    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 60;  


    ApplyEffect(obj,{f1});
    PushObj(GetX(GetObjRef()),GetY(GetObjRef()),obj,90,0.1)
    PlaySound("assets/enemies/shield_warrior/audio/shield_stance.wav",1);
    ShakeScreen(2,0.1)
    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
end