
function setup()
    AbilitySetPortrait("assets/friendly/ranger/aimed_shot.png");
    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetDescription("Aimed Shot\n\nA heavy shot. Generates a stack of Flurry.")
    SetCooldown(8);
    SetAbilityRange(50);
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/rogue/audio/cloak.wav",1.25)

    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT
    f1["type"] = EFFECT_HURT
    f1["value"] = 140;
    ApplyEffect(obj,{f1})
    AddStack(GetObjRef(),2,1);


    return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end