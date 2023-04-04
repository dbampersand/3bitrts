
function setup()
    AbilitySetPortrait("assets/friendly/monk/icon_palm.png");
    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetDescription("Palm\n\nA heavy hit which cleaves to nearby units. Generates a chi.")
    SetCooldown(6);
    SetAbilityRange(50);    
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/rogue/audio/cloak.wav",1.25)

    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT
    f1["type"] = EFFECT_HURT
    f1["value"] = 100;
    CleaveEffect(obj,{f1},2,60,0.5);

    AddStack(GetObjRef(),2,1);
    AddStack(GetObjRef(),3,1);
    AddStack(GetObjRef(),4,1);


    ShakeScreen(2,0.1)
    return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end